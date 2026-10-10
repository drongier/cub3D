#ifndef ENEMY_H
# define ENEMY_H

# include <stdbool.h>
# include <stdint.h>
# include "grid.h"
# include "level.h"
# include "motion.h"
# include "raycast.h"

/* Mutant : positions et distances en cases, temps en secondes */
# define ENEMY_HP 50
# define ENEMY_SPEED 1.6f
# define ENEMY_RADIUS 0.3f
/* Distance minimale gardée avec le joueur */
# define ENEMY_KEEP_AWAY 0.8f
/* Rayon touché par une balle autour du centre du sprite */
# define ENEMY_HIT_RADIUS 0.3f
# define ENEMY_SIGHT 16.0f
/* Demi-angle de vision, en radians (un peu plus que l'avant) */
# define ENEMY_SIGHT_HALF_ANGLE 1.75f
# define ENEMY_HEARING 8.0f
# define ENEMY_ATTACK_RANGE 10.0f
/* Délai avant la première attaque, puis entre deux attaques (+ hasard) */
# define ENEMY_REACTION 0.5
# define ENEMY_ATTACK_PAUSE 1.2
/* Attaque : 4 images, deux tirs */
# define ENEMY_SHOT_1 0.25
# define ENEMY_SHOT_2 0.60
# define ENEMY_ATTACK_END 0.85
# define ENEMY_PAIN_TIME 0.2
# define ENEMY_DEATH_FRAME 0.12
/* Dégâts : du mutant au joueur, du pistolet au mutant (min + hasard) */
# define ENEMY_DAMAGE_MIN 5
# define ENEMY_DAMAGE_RAND 11
# define PISTOL_DAMAGE_MIN 20
# define PISTOL_DAMAGE_RAND 16
/*
 * Vagues : places réservées en plus des 'M' quand la scène a des points
 * d'apparition. Une fois pleines, les corps les plus anciens sont recyclés.
 */
# define HORDE_WAVE_SLOTS 96
/* Un point d'apparition est choisi à au moins tant de cases de marche */
# define SPAWN_MIN_STEPS 6
/* Sinon, faute de mieux, même en vue, mais jamais plus près que ça */
# define SPAWN_FALLBACK_STEPS 3

enum e_enemy_state
{
	EN_IDLE,
	EN_CHASE,
	EN_ATTACK,
	EN_PAIN,
	EN_DYING,
	EN_DEAD
};

typedef struct s_enemy
{
	t_vec2	pos;
	float	facing;
	int		state;
	double	t;
	double	walk_t;
	double	cooldown;
	bool	moving;
	int		hp;
	float	speed;
}	t_enemy;

/*
 * Tous les ennemis d'une scène. flow : distance en cases de chaque case
 * jusqu'au joueur (-1 : mur ou inaccessible), recalculée quand le joueur
 * change de case. v a cap places, dont n utilisées ; reuse : où reprendre la
 * recherche d'un corps à recycler.
 */
typedef struct s_horde
{
	t_enemy	*v;
	int		n;
	int		cap;
	int		reuse;
	int		*flow;
	int		*queue;
	int		flow_cell;
	t_vec2	player;
	uint32_t	rng;
}	t_horde;

/* Ce que les ennemis savent du joueur pendant une frame */
typedef struct s_target
{
	t_vec2	pos;
	bool	alive;
	bool	fired;
}	t_target;

/* Case de la planche : colonne 0 à 7, rangée 0 à 6 */
typedef struct s_cell
{
	int	col;
	int	row;
}	t_cell;

bool	horde_init(t_horde *h, const t_level *lv);
void	horde_free(t_horde *h);
/* Avance tout le monde de dt ; renvoie les dégâts infligés au joueur */
int		horde_update(t_horde *h, const t_grid *g, t_target target, double dt);
/* Tir du joueur depuis pos vers angle ; renvoie l'ennemi touché ou -1 */
int		horde_shoot(t_horde *h, const t_grid *g, t_vec2 pos, float angle);
/* Un ennemi vivant occupe-t-il le disque (p, radius) ? */
bool	horde_blocks(const t_horde *h, t_vec2 p, float radius);
int		horde_alive(const t_horde *h);

/* Ce qui change d'une vague à l'autre pour un nouveau mutant */
typedef struct s_breed
{
	int		hp;
	float	speed;
}	t_breed;

/*
 * Choisit un point d'apparition loin du joueur (en cases de marche), hors de
 * sa vue et libre ; false si aucun ne convient pour l'instant. Le champ de
 * distances doit être à jour (horde_update l'a calculé).
 */
bool	horde_pick_spawn(t_horde *h, const t_grid *g, const t_level *lv,
			t_vec2 *out);
/* Ajoute un mutant qui fonce sur le joueur ; false si la horde est pleine */
bool	horde_spawn(t_horde *h, t_vec2 pos, t_breed breed);

/* Distance de vue dégagée : aucun mur entre a et b */
bool	clear_line(const t_grid *g, t_vec2 a, t_vec2 b);
void	flow_update(t_horde *h, const t_grid *g, t_vec2 player);
/* Image à afficher pour un ennemi vu depuis viewer */
t_cell	enemy_cell(const t_enemy *e, t_vec2 viewer);
uint32_t	horde_rand(t_horde *h);

/* Internes à l'IA, exposées pour les tests */
void	enemy_move(t_horde *h, int i, const t_grid *g, t_vec2 player,
			double dt);

#endif
