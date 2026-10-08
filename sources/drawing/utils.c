/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:05:39 by drongier          #+#    #+#             */
/*   Updated: 2025/05/05 12:02:57 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* 1) Convert pixel to block to check 2D map
	2) Return true if ray hit wall 
*/
bool	touch(float px, float py, t_game *game)
{
	return (grid_at(&game->grid, (int)floorf(px / BLOCK),
			(int)floorf(py / BLOCK)) == CELL_WALL);
}
