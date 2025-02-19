/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:17:47 by drongier          #+#    #+#             */
/*   Updated: 2025/02/19 16:18:31 by drongier         ###   ########.fr       */
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
void draw_map(t_game *game)
{
	int x;
	int y;
	char **map = game->map;
	int color = 0xFF00FF; //color of the map, ici pink
	
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == '1')
				draw_square(x * BLOCK, y * BLOCK, BLOCK, color, game);
			x++;
		}
		y++;
	}
}