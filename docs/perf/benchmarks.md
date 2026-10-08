# Benchmarks cub3D

Machine : Apple M1 Pro (MacBookPro18,3), macOS 15.5, Apple clang 17, SDL3 3.4.12.
Commande : `./cub3D --bench 1000 <map>` (vsync off, 1280x720, minimap active, rotation
de 0.03 rad par frame sur place). Chaque ligne est le run médian sur 3.

| Étape | Build | Map | render avg | render p99 | frame avg | frame p99 | fps |
|---|---|---|---|---|---|---|---|
| 1. Portage SDL3 | -O0 | cheese_maze | 11.94 | 13.10 | 12.57 | 14.19 | 79.6 |
| 1. Portage SDL3 | -O0 | square_map | 16.11 | 23.73 | 17.78 | 25.18 | 56.2 |
| 1. Portage SDL3 | -O2 | cheese_maze | 3.58 | 4.44 | 4.16 | 6.05 | 240.2 |
| 1. Portage SDL3 | -O2 | square_map | 4.94 | 7.05 | 5.50 | 7.65 | 181.7 |
| 2. Boucle de jeu | -O0 | cheese_maze | 11.88 | 13.40 | 12.42 | 14.28 | 80.5 |
| 2. Boucle de jeu | -O0 | square_map | 16.06 | 23.33 | 17.60 | 24.07 | 56.8 |
| 2. Boucle de jeu | -O2 | cheese_maze | 3.67 | 5.14 | 6.82 | 9.28 | 146.7 |
| 2. Boucle de jeu | -O2 | square_map | 4.98 | 7.28 | 5.62 | 8.65 | 177.9 |

Étape 2 : le rendu ne change pas (même image, le déplacement sort juste de `draw_loop`). Les
écarts de `frame avg` viennent de blocages d'environ 1 s dans `platform_present` (colonne `max`
des runs bruts, jusqu'à 1060 ms), présents aussi à l'étape 1 et sans lien avec le code de jeu :
à surveiller, `render` est la colonne fiable pour comparer les étapes.
