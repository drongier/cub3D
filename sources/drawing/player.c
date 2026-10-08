/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:49:58 by drongier          #+#    #+#             */
/*   Updated: 2025/05/08 18:07:39 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* Teste la marge devant le joueur, axe par axe, pour glisser le long des murs */
static void	check_collision(t_player *player, t_vec2 step)
{
	float	len;
	float	next_x;
	float	next_y;

	len = sqrtf(step.x * step.x + step.y * step.y);
	if (len == 0.0f)
		return ;
	next_x = player->x + step.x / len * COLLISION_MARG;
	next_y = player->y + step.y / len * COLLISION_MARG;
	if (!touch(next_x, player->y, player->game))
		player->x += step.x;
	if (!touch(player->x, next_y, player->game))
		player->y += step.y;
}

void	update_player(t_player *player, double dt)
{
	int	forward;
	int	strafe;
	int	turn;

	turn = player->right_rotate - player->left_rotate;
	forward = player->key_up - player->key_down;
	strafe = player->key_right - player->key_left;
	player->angle = motion_turn(player->angle, turn, dt);
	check_collision(player, motion_step(player->angle, forward, strafe, dt));
}
