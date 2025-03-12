/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:05:39 by drongier          #+#    #+#             */
/*   Updated: 2025/03/12 18:01:25 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// our own clear_image
// void	clear_image(t_game *game)
// {
// 	int	x;
// 	int	y;

// 	y = 0;
// 	while (y < HEIGHT)
// 	{
// 		x = 0;
// 		while (x < WIDTH)
// 		{
// 			put_pixel(x, y, 0, game);
// 			x++;
// 		}
// 		y++;
// 	}
// }

/* 1) Convert pixel to block to check 2D map
	2) Return true if ray hit wall 
*/
bool	touch(float px, float py, t_game *game)
{
	int	x;
	int	y;
	char	**map;

	map = game->map->map;
	x = px / BLOCK;
	y = py / BLOCK;
	if (map[y][x] == '1')
		return (true);
	return (false);
}

// void	get_size_map(t_game *game)
// {
// 	int	row_length;

// 	game->map_width = 0;
// 	game->map_height = 0;
// 	while (game->map[game->map_height])
// 	{
// 		row_length = 0;
// 		while (game->map[game->map_height][row_length])
// 			row_length++;
// 		if (row_length > game->map_width)
// 			game->map_width = row_length;
// 		game->map_height++;
// 	}
// }

int	close_window(t_game *game)
{
	mlx_destroy_window(game->mlx, game->win);
	exit(0);
	return (0);
}
