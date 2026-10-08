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
