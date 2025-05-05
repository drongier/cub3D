/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:30:33 by drongier          #+#    #+#             */
/*   Updated: 2025/05/05 12:07:53 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// distance calculation functions
float	distance(float x, float y)
{
	return (sqrt(x * x + y * y));
}

/* Removing distortion + fisheyes effect
 /!\ MATH /!\ */
float	fixed_dist(t_player *player, float x2, float y2, t_game *game)
{
	float	delta_x;
	float	delta_y;
	float	angle;
	float	fix_dist;

	delta_x = x2 - player->x;
	delta_y = y2 - player->y;
	angle = atan2(delta_y, delta_x) - game->player.angle;
	fix_dist = distance(delta_x, delta_y) * cos(angle);
	return (fix_dist);
}

/* Define height of wall by distance between player and next wall */
float	calculate_height(t_player *player, float ray_x, float ray_y)
{
	float	dist;
	float	height;

	dist = fixed_dist(player, ray_x, ray_y, player->game);
	height = (BLOCK / dist) * (WIDTH / 2);
	return (height);
}
