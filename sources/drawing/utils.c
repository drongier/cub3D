/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:05:39 by drongier          #+#    #+#             */
/*   Updated: 2025/03/21 10:10:36 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* 1) Convert pixel to block to check 2D map
	2) Return true if ray hit wall 
*/
bool	touch(float px, float py, t_game *game)
{
	int		x;
	int		y;
	char	**map;

	map = game->map->map;
	x = px / BLOCK;
	y = py / BLOCK;
	if (map[y][x] == '1')
		return (true);
	return (false);
}

int	close_window(t_game *game)
{
	mlx_destroy_window(game->mlx, game->win);
	exit(0);
	return (0);
}
