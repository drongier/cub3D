/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_mouv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:55:52 by drongier          #+#    #+#             */
/*   Updated: 2025/03/11 14:41:39 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	m_up(t_player *player, float cos_angle, float sin_angle, int speed)
{
	player->x += cos_angle * speed;
	player->y += sin_angle * speed;
}

void	m_down(t_player *player, float cos_angle, float sin_angle, int speed)
{
	player->x -= cos_angle * speed;
	player->y -= sin_angle * speed;
}

void	m_left(t_player *player, float cos_angle, float sin_angle, int speed)
{
	player->x += sin_angle * speed;
	player->y -= cos_angle * speed;
}

void	m_right(t_player *player, float cos_angle, float sin_angle, int speed)
{
	player->x -= sin_angle * speed;
	player->y += cos_angle * speed;
}

void	check_boundaries(t_player *player)
{
    // Effet miroir horizontal
	if (player->x >= player->game->map_width * BLOCK)
		player->x = 0;
	else if (player->x < 0)
		player->x = (player->game->map_width - 1) * BLOCK;
    // Effet miroir vertical
	if (player->y >= player->game->map_height * BLOCK)
		player->y = 0;
	else if (player->y < 0)
		player->y = (player->game->map_height - 1) * BLOCK;
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
