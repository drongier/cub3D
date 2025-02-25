/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:05:39 by drongier          #+#    #+#             */
/*   Updated: 2025/02/25 15:55:59 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// our own clear_image
void clear_image(t_game *game)
{
	int x;
	int y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			put_pixel(x, y, 0, game);
			x++;
		}
		y++;
	}
}

bool touch(float px, float py, t_game *game)
{
    int x = px / BLOCK;
    int y = py / BLOCK;
    if(game->map[y][x] == '1')
        return true;
    return false;
}

void get_size_map(t_game *game)
{
	game->map_width = 0;
    game->map_height = 0;
    while (game->map[game->map_height])
    {
        int row_length = 0;
        while (game->map[game->map_height][row_length])
            row_length++;
        if (row_length > game->map_width)
            game->map_width = row_length;
        game->map_height++;
    }
}