/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:22:08 by drongier          #+#    #+#             */
/*   Updated: 2025/05/06 18:18:57 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_west_east(float start_x)
{
	if (cos(start_x) > 0)
		return (EAST);
	else
		return (WEST);
}

int	is_nord_sud(float start_x)
{
	if (sin(start_x) > 0)
		return (SUD);
	else
		return (NORD);
}

void	update_angle(t_player *player, float angle_speed)
{
	if (player->left_rotate)
		player->angle -= angle_speed;
	if (player->right_rotate)
		player->angle += angle_speed;
	if (player->angle > 2 * PI)
		player->angle -= 2 * PI;
	if (player->angle < 0)
		player->angle += 2 * PI;
}

int	get_cell_index(float coord)
{
	return ((int)coord / BLOCK);
}

int	is_diagonal(int curr_x, int curr_y, int next_x, int next_y)
{
	return (curr_x != next_x && curr_y != next_y);
}
