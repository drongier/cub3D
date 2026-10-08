# Sous-projet 1 — Portage SDL3 et outils de mesure

Date : 2026-10-08
Branche : `perf/01-sdl3-port`

## Contexte

cub3D a été rendu et validé à 42. On le reprend hors du cadre 42 (plus de Norminette,
plus de restriction de fonctions) avec un objectif : la fluidité et la performance maximales.
L'audit a donné une feuille de route en sous-projets, chacun suivi d'un test par l'utilisateur
avant de passer au suivant :

1. **Portage SDL3 + mesure** (ce document)
2. Boucle de jeu : delta time, rythme des frames, déplacement normalisé
3. Minimap : réutiliser les impacts du rendu 3D, fond pré-rendu
4. Raycaster : plan caméra, `t_hit`, map plate, projection corrigée
5. Boucle de pixels : `uint32_t`, clipping, fond ligne par ligne, textures en colonnes
6. Parsing et fuites restantes
7. (optionnel) Threads, résolution interne réduite

Plateformes : macOS arm64 en priorité (SDL3 3.4 via Homebrew), Linux ensuite.

## Objectif

Le jeu compile et tourne nativement sur macOS et Linux avec SDL3, **avec un comportement
identique à aujourd'hui** : même image, même vitesse par frame, mêmes touches. On ajoute de quoi
mesurer. Aucune optimisation du rendu dans ce sous-projet : il fixe la référence chiffrée.

## Hors périmètre

Delta time, souris, plein écran, toute modification de `draw_loop`, du raycaster, de la
minimap ou du parseur (sauf ce qui est imposé par la suppression de minilibx).

## Architecture

### Couche plateforme — `sources/platform/platform_sdl.c`

Seul fichier qui inclut `<SDL3/SDL.h>`. Interface déclarée dans `includes/platform.h`, qui
n'expose aucun type SDL (pointeurs opaques) :

```c
typedef struct s_input
{
	bool	up;          /* W */
	bool	down;        /* S */
	bool	left;        /* A */
	bool	right;       /* D */
	bool	rot_left;    /* flèche gauche */
	bool	rot_right;   /* flèche droite */
	bool	quit;        /* Échap ou fermeture de la fenêtre */
}	t_input;

typedef struct s_platform t_platform;  /* opaque */

t_platform	*platform_init(int width, int height, bool vsync);
void		platform_poll(t_platform *p, t_input *in);
void		platform_present(t_platform *p, const uint32_t *fb);
void		platform_set_title(t_platform *p, const char *title);
uint64_t	platform_ticks(void);        /* compteur haute précision */
double		platform_ticks_to_ms(uint64_t ticks);
void		platform_destroy(t_platform *p);
```

- `platform_init` : `SDL_Init(SDL_INIT_VIDEO)`, fenêtre 1280×720 non redimensionnable,
  `SDL_Renderer`, texture `SDL_PIXELFORMAT_XRGB8888` en `SDL_TEXTUREACCESS_STREAMING`,
  `SDL_SetRenderVSync(renderer, vsync ? 1 : 0)`. Retourne `NULL` en cas d'échec après avoir
  libéré ce qui a été créé et affiché `SDL_GetError()`.
- `platform_poll` : vide la file d'événements. `KEY_DOWN`/`KEY_UP` mettent à jour les booléens
  (les répétitions automatiques, `event.key.repeat`, sont ignorées). `SDL_EVENT_QUIT` et Échap
  mettent `quit` à vrai.
- `platform_present` : `SDL_UpdateTexture` (pitch = largeur × 4), `SDL_RenderTexture`,
  `SDL_RenderPresent`.
- Le jeu ne voit jamais un événement ou un type SDL.

### Framebuffer

`uint32_t *fb` de WIDTH×HEIGHT, alloué dans `init_game`. Pour ce sous-projet, `game->data`,
`game->size_line` (= WIDTH × 4) et `game->bpp` (= 32) pointent dessus : `put_pixel` et tout le
code de dessin restent inchangés. L'ordre d'octets écrit par `put_pixel` (B, G, R, x) correspond
à `XRGB8888` en little-endian, ce qui couvre arm64 et x86_64.

### Lecteur XPM — `sources/platform/xpm_loader.c`

```c
bool	xpm_load(const char *path, t_texture *out);
void	texture_free(t_texture *t);
```

- Lit le fichier en entier, ne garde que les chaînes entre guillemets.
- Première chaîne : `largeur hauteur ncolors cpp` (cpp = 1 ou 2 ; le reste est refusé).
- `ncolors` lignes de couleur : clé de `cpp` caractères, puis le token `c` suivi de
  `#RRGGBB`, `#RGB` ou `None` (transparent → 0x000000 pour l'instant). Une couleur nommée
  ou un format inconnu provoque un échec.
- Puis `hauteur` lignes de `largeur × cpp` caractères. Table de correspondance :
  tableau direct de 256 entrées si cpp = 1, de 65536 entrées si cpp = 2.
- Remplit `t_texture` : `data` pointe vers un `uint32_t[width * height]` alloué,
  `size_line = width * 4`, `bpp = 32`, `img = NULL`. Le code de dessin (`get_texture_color`)
  reste inchangé.
- Toute erreur (fichier, en-tête, couleur, ligne trop courte) → `false`, rien n'est alloué.
  `init_textures` appelle alors `ft_error` après avoir libéré les textures déjà chargées.

### Boucle principale — `sources/main.c`

```
parse args → parse scene → init_game (fb, textures, player) → platform_init
while (!input.quit):
    t0 = ticks
    platform_poll(&input)
    copier input → flags du player (key_up, key_down, ...)
    t1 = ticks
    draw_loop(game)            /* appelle move_player puis dessine, inchangé */
    t2 = ticks
    platform_present(fb)
    t3 = ticks
    stats : render = t2 - t1, frame = t3 - t0
cleanup
```

- `draw_loop` perd son appel à `mlx_put_image_to_window`, rien d'autre.
- `key_press`, `key_release` et `close_window` sont supprimés (remplacés par `t_input`).
- Le joueur bouge toujours de 3 px et tourne de 0.03 rad **par frame**, comme avant. Avec la
  vsync, sur un écran 60 Hz, la vitesse ressentie est celle d'origine ; sur un écran 120 Hz,
  elle double. C'est connu et accepté : le sous-projet 2 introduit le delta time.

### Sortie et libération

Un seul chemin de sortie après la boucle : textures (`texture_free` ×4), framebuffer,
`platform_destroy` (texture SDL, renderer, fenêtre, `SDL_Quit`), puis `ft_cleanup_map`.
`exit_game` est remplacé par ce chemin. Les erreurs de parsing gardent `ft_error` (exit 1),
inchangé.

### Suppressions

- Le répertoire `minilibx-linux/` (il reste dans l'historique git).
- `#include <bits/types.h>` et `#include "../minilibx-linux/mlx.h"` dans `cub3d.h`.
- Les champs `mlx`, `win`, `img`, `endian` de `t_game` ; `img` et `endian` de `t_texture`
  deviennent inutiles (`img` est retiré, `endian` aussi).
- Les fonctions mortes listées par l'audit restent pour le sous-projet concerné, sauf celles
  qui ne compilent plus.

## Build — `Makefile`

- `CFLAGS = -Wall -Wextra -Werror -MMD -MP $(OPT) -g`, avec `OPT ?= -O2`.
- SDL3 : `$(shell pkg-config --cflags sdl3)` et `$(shell pkg-config --libs sdl3)`.
  Si `pkg-config` ne trouve pas sdl3, le Makefile s'arrête avec un message qui indique la
  commande d'installation (`brew install sdl3` / paquet `libsdl3-dev` / compilation depuis
  les sources).
- Objets dans `build/` (miroir de l'arborescence `sources/`), dépendances `.d` incluses.
- Cibles : `all` (défaut), `debug` (`OPT=-O0`, ajoute `-fsanitize=address,undefined` à la
  compilation et à l'édition de liens, sortie dans `build-debug/` pour ne pas mélanger les
  objets), `clean`, `fclean`, `re`. `.PHONY` corrigé. libft reste construite par son propre
  Makefile.
- `make OPT=-O0` reproduit le niveau d'optimisation du build d'origine.
- `.gitignore` : ajouter `build/`, `build-debug/`, retirer la ligne minilibx.

## Mesure

### Ligne de commande

`./cub3D [--no-vsync] [--bench [N]] <scene.cub>`

- Les options peuvent être dans n'importe quel ordre ; exactement un argument non-option,
  le fichier `.cub`. Option inconnue ou fichier manquant → message d'usage, exit 1.
- `--no-vsync` : vsync désactivée.
- `--bench [N]` : N frames (défaut 1000, N > 0), vsync forcée à désactivée, entrées clavier
  ignorées sauf Échap/fermeture. Chaque frame simule « flèche droite enfoncée » : le joueur
  reste au point de départ et tourne de 0.03 rad par frame. Minimap active (`BONUS == 1`).

### Compteur en direct

Toutes les 500 ms, titre de la fenêtre :
`cub3D | <fps> fps | frame <ms> ms | render <ms> ms` (moyennes sur la fenêtre de 500 ms,
une décimale pour les ms). Aucun affichage terminal en mode normal.

### Rapport de bench

Les durées render et frame de chaque frame sont stockées dans deux tableaux de N `double`.
En fin de bench, sortie sur stdout puis exit 0 :

```
bench: maps/good/cheese_maze.cub, 1000 frames, 1280x720, vsync off
         avg     median  p99     min     max     (ms)
render   2.13    2.10    2.71    2.02    3.40
frame    2.45    2.41    3.10    2.30    4.02
fps (avg frame): 408.2
```

p99 = valeur au rang `ceil(0.99 × N) - 1` du tableau trié.

### Suivi — `docs/perf/benchmarks.md`

Tableau par sous-projet sur deux maps de référence : `maps/good/cheese_maze.cub` et
`maps/good/square_map.cub` (la plus grande, 42 lignes × 64 colonnes). Pour ce sous-projet :
deux lignes, `OPT=-O0` (référence d'origine) et `-O2`. Colonnes : render avg/p99, frame avg/p99,
fps. Machine notée en tête (modèle, OS).

## Tests

- **`test.sh`** : il dépend de valgrind, absent sur macOS arm64. Il est adapté : si
  `valgrind` est présent, comportement actuel ; sinon il lance `./cub3D` directement, et
  considère le cas réussi si le code de sortie est 1 et que la sortie contient `Error`.
  Un crash (code ≥ 128) est signalé ❌. Le script finit par un résumé `X/Y ok` et un code de
  sortie non nul en cas d'échec.
- **Lecteur XPM** : `tests/xpm_test.c` + cible `make test`. Charge les 51 `.xpm` du dépôt et
  vérifie dimensions et valeur d'un pixel connu pour deux fichiers ; vérifie l'échec propre
  sur un fichier inexistant, sur un fichier sans droit de lecture (créé par le test, `chmod
  000`) et sur un fichier XPM tronqué créé par le test. (`textures/test/forbidden.xpm` ne sert
  pas : git ne conserve pas l'absence de droit de lecture, le fichier est lisible.) Pas de framework : un `main` qui compte les échecs.
- **Fuites sur macOS** : `leaks --atExit -- ./cub3D --bench 200 maps/good/cheese_maze.cub`
  doit rapporter 0 fuite (le leak checker d'ASan n'est pas disponible sur macOS arm64).
- **Comparaison visuelle** : pas de capture automatique d'image dans ce sous-projet ; la
  validation visuelle est faite par l'utilisateur.

## Validation par l'utilisateur (fin du sous-projet)

1. `make` compile sans warning sur le Mac.
2. `./test.sh` : toutes les maps invalides sont refusées comme avant.
3. Le jeu se lance sur plusieurs maps de `maps/good` : textures, couleurs et minimap
   identiques, WASD et flèches fonctionnent, Échap et la croix ferment proprement.
4. `./cub3D --bench 1000 maps/good/cheese_maze.cub` affiche le rapport.
5. `make debug` puis une partie rapide : aucune erreur ASan/UBSan.
6. `docs/perf/benchmarks.md` contient la référence O0 et O2.

## Risques

- **Comportement par frame** : sur un écran 120 Hz avec vsync, le jeu va 2× plus vite qu'à
  60 Hz. Accepté, corrigé au sous-projet 2.
- **Ordre des octets** : `XRGB8888` suppose little-endian (arm64, x86_64). Pas de cible
  big-endian prévue.
- **Linux** : `libsdl3-dev` n'existe que sur les distributions récentes ; sinon SDL3 se
  compile depuis les sources. Documenté dans le README.
- **README** : les sections Build, Run et Keys sont mises à jour (SDL3, nouvelles options).
