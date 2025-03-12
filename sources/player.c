/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:49:58 by drongier          #+#    #+#             */
/*   Updated: 2025/03/12 19:15:27 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_player(t_player *player, t_map *map)
{
	player->o = map->player_o;
	// player->x = WIDTH / 2;
	// player->y = HEIGHT / 2;
	player->x = (float)map->player_x * BLOCK + BLOCK/2;
	player->y = (float)map->player_y * BLOCK + BLOCK/2;
	// player->x = 1008;
	// player->y = 565;
	printf("x: %f\n", player->x);
	printf("y: %f\n", player->y);
	player->angle = 3 * PI / 2;
	player->hit_dir = -1;
	player->key_up = false;
	player->key_down = false;
	player->key_right = false;
	player->key_left = false;
	player->key_exit = false;
	player->left_rotate = false;
	player->right_rotate = false;
	player->game = NULL;
}

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
		m_up(player, cos_angle, sin_angle, speed);
	if (player->key_down)
		m_down(player, cos_angle, sin_angle, speed);
	if (player->key_left)
		m_left(player, cos_angle, sin_angle, speed);
	if (player->key_right)
		m_right(player, cos_angle, sin_angle, speed);
	if (BONUS == 1)
		check_boundaries(player);
	else
		check_boundaries(player);
}
