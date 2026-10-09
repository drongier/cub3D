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

/*
 * Teste un point devant le joueur, axe par axe, pour glisser le long des
 * murs. Le point est à COLLISION_MARG, ou plus loin si le pas est plus long
 * (sprint à bas fps), pour ne jamais finir dans un mur.
 */
static void	check_collision(t_player *player, t_vec2 step)
{
	float	len;
	float	probe;
	float	next_x;
	float	next_y;

	len = sqrtf(step.x * step.x + step.y * step.y);
	if (len == 0.0f)
		return ;
	probe = fmaxf(COLLISION_MARG, len);
	next_x = player->x + step.x / len * probe;
	next_y = player->y + step.y / len * probe;
	if (!touch(next_x, player->y, player->game))
		player->x += step.x;
	if (!touch(player->x, next_y, player->game))
		player->y += step.y;
}

/* Touches de déplacement enfoncées, combinées */
t_move	player_move(const t_player *player)
{
	return ((t_move){player->key_up - player->key_down,
		player->key_right - player->key_left, player->sprint});
}

void	update_player(t_player *player, double dt)
{
	player->angle = motion_turn(player->angle,
			player->right_rotate - player->left_rotate, dt);
	check_collision(player, motion_step(player->angle, player_move(player),
			dt));
}
