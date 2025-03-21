/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:49:58 by drongier          #+#    #+#             */
/*   Updated: 2025/03/21 11:44:57 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	player_pos(t_player *player, int angle_speed)
{
	if (player->left_rotate)
		player->angle -= angle_speed;
	if (player->right_rotate)
		player->angle += angle_speed;
	if (player->angle > 2 * PI)
		player->angle = 0;
	if (player->angle < 0)
		player->angle = 2 * PI;
}

void	check_collision(t_player *player, float dx, float dy, int speed)
{
	float	next_x;
	float	next_y;

	next_x = player->x + dx * COLLISION_MARG;
	next_y = player->y + dy * COLLISION_MARG;
	if (!touch(next_x, player->y, player->game))
		player->x += dx * speed;
	if (!touch(player->x, next_y, player->game))
		player->y += dy * speed;
}

void	move_player(t_player *player)
{
	int		speed;
	float	angle_speed;
	float	cos_angle;
	float	sin_angle;

	speed = 3;
	angle_speed = 0.03;
	cos_angle = cos(player->angle);
	sin_angle = sin(player->angle);
	update_angle(player, angle_speed);
	if (player->key_up)
		check_collision(player, cos_angle, sin_angle, speed);
	if (player->key_down)
		check_collision(player, -cos_angle, -sin_angle, speed);
	if (player->key_left)
		check_collision(player, sin_angle, -cos_angle, speed);
	if (player->key_right)
		check_collision(player, -sin_angle, cos_angle, speed);
}
