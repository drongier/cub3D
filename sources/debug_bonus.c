/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:17:47 by drongier          #+#    #+#             */
/*   Updated: 2025/02/25 14:22:16 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// ONLY DEBUG MODE or MINI MAP BONUS WIP
void draw_square(int x, int y, int size, int color, t_game *game)
{
	int i;
	
	i = 0;
	while (i++ < size)
		put_pixel(x + i, y, color, game);
	i = 0;
	while (i++ < size)
		put_pixel(x, y + i, color, game);
	i = 0;
	while (i++ < size)
		put_pixel(x + size, y + i, color, game);
	i = 0;
	while (i++ < size)
		put_pixel(x + i, y + size, color, game);
}
// ONLY DEBUG MODE or MINI MAP BONUS WIP
void draw_map(t_game *game, int offset_x, int offset_y)
{
    int x;
    int y;
    char **map = game->map;
    int color = 0x000000; // color of the map, ici pink

    y = 0;
    while (map[y])
    {
        x = 0;
        while (map[y][x])
        {
            if (map[y][x] == '1')
                draw_square_full(offset_x + x * BLOCK / 4, offset_y + y * BLOCK / 4, BLOCK / 4, color, game);
            x++;
        }
        y++;
    }
}

void draw_target(t_game *game)
{
    // Calculer la taille de la carte
    int map_width = 0;
    int map_height = 0;
    while (game->map[map_height])
    {
        int row_length = 0;
        while (game->map[map_height][row_length])
            row_length++;
        if (row_length > map_width)
            map_width = row_length;
        map_height++;
    }

    // Définir la taille du carré en fonction de la taille de la carte
    int square_size = (map_width > map_height ? map_width : map_height) * BLOCK / 4;

    // Dessiner un carré rouge en bas à droite de l'écran
    int bottom_right_x = WIDTH - square_size;
    int bottom_right_y = HEIGHT - square_size;
    // draw_square_full(bottom_right_x, bottom_right_y, square_size, 0x000000, game);

    // Dessiner la carte à l'intérieur du carré
    draw_map(game, bottom_right_x, bottom_right_y);
    int player_x = bottom_right_x + (game->player.x / BLOCK) * (BLOCK / 4);
    int player_y = bottom_right_y + (game->player.y / BLOCK) * (BLOCK / 4);
    draw_square_full(player_x, player_y, 5, 0x00FF00, game);
}