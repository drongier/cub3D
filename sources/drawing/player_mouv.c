/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_mouv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:55:52 by drongier          #+#    #+#             */
/*   Updated: 2025/05/09 16:23:08 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	m_up(t_player *player, int cos_angle, int sin_angle, int speed)
{
	player->x += cos_angle * speed;
	player->y += sin_angle * speed;
}

void	m_down(t_player *player, int cos_angle, int sin_angle, int speed)
{
	player->x -= cos_angle * speed;
	player->y -= sin_angle * speed;
}

void	m_left(t_player *player, int cos_angle, int sin_angle, int speed)
{
	player->x += sin_angle * speed;
	player->y -= cos_angle * speed;
}

void	m_right(t_player *player, int cos_angle, int sin_angle, int speed)
{
	player->x -= sin_angle * speed;
	player->y += cos_angle * speed;
}

void	check_boundaries(t_player *player)
{
	int	next_x;
	int	next_y;

	next_x = player->x + cos(player->angle) * 5;
	next_y = player->y + sin(player->angle) * 5;
	if (touch(next_x, player->y, player->game))
		player->x -= cos(player->angle) * 5;
	if (touch(player->x, next_y, player->game))
		player->y -= sin(player->angle) * 5;
}
