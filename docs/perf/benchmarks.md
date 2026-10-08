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
| 3. Minimap | -O0 | cheese_maze | 10.83 | 12.63 | 11.41 | 13.48 | 87.6 |
| 3. Minimap | -O0 | square_map | 5.63 | 6.34 | 6.20 | 7.17 | 161.4 |
| 3. Minimap | -O2 | cheese_maze | 3.14 | 3.75 | 3.75 | 6.00 | 266.6 |
| 3. Minimap | -O2 | square_map | 1.73 | 2.03 | 2.40 | 8.69 | 416.2 |
| 4. Raycaster | -O0 | cheese_maze | 7.83 | 8.44 | 8.41 | 9.44 | 118.9 |
| 4. Raycaster | -O0 | square_map | 5.83 | 6.70 | 6.40 | 7.51 | 156.3 |
| 4. Raycaster | -O2 | cheese_maze | 2.39 | 4.19 | 3.16 | 8.43 | 316.7 |
| 4. Raycaster | -O2 | square_map | 1.76 | 2.24 | 3.27 | 3.27 | 305.4 |

Étape 2 : le rendu ne change pas (même image, le déplacement sort juste de `draw_loop`). Les
écarts de `frame avg` viennent de blocages d'environ 1 s dans `platform_present` (colonne `max`
des runs bruts, jusqu'à 1060 ms), présents aussi à l'étape 1 et sans lien avec le code de jeu :
à surveiller, `render` est la colonne fiable pour comparer les étapes.

Étape 3 : l'ancienne minimap relançait 1280 rayons pixel par pixel et redessinait toute la map
(512x336 sur square_map) : 0.6 ms sur cheese_maze, 3.3 ms sur square_map (65 % du rendu). Le
radar de taille fixe réutilise les impacts du rendu 3D et coûte environ 0.1 ms ; le rendu
avec radar est à 0.1 ms près celui d'un build sans minimap (`BONUS 0`).

Étape 4 : plus de trigonométrie par colonne (plan caméra, distance perpendiculaire donnée par le
DDA) et une grille plate. Champ de vision passé de 60° à 66° avec la focale correspondante : les
murs sont environ 1.5x plus hauts à l'écran, donc plus de pixels texturés par image. Malgré cela,
cheese_maze passe de 3.14 à 2.39 ms ; square_map, dont le coût est surtout le remplissage des
pixels, reste à 1.76 ms (sous-projet 5). Le `frame avg` de square_map -O2 est gonflé par deux
blocages d'environ 1 s dans `platform_present` sur ce run.
