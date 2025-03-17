/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:49:58 by drongier          #+#    #+#             */
/*   Updated: 2025/03/17 13:11:10 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_player(t_player *player, t_map *map)
{
	player->o = map->player_o;
	player->x = (float)map->player_x * BLOCK + BLOCK/2;
	player->y = (float)map->player_y * BLOCK + BLOCK/2;
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

void check_collision(t_player *player, float dx, float dy, int speed)
{
    float next_x;
    float next_y;
	
	next_x = player->x + dx * COLLISION_MARG;
    next_y = player->y + dy * COLLISION_MARG;
    if (!touch(next_x, player->y, player->game))
        player->x += dx * speed;
    if (!touch(player->x, next_y, player->game))
        player->y += dy * speed;
}

void move_player(t_player *player)
{
    int     speed = 3;
    float   angle_speed = 0.03;
    float   cos_angle = cos(player->angle);
    float   sin_angle = sin(player->angle);

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