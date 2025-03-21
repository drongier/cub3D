/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:30:33 by drongier          #+#    #+#             */
/*   Updated: 2025/03/21 14:01:58 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// distance calculation functions
float	distance(float x, float y)
{
	return (sqrt(x * x + y * y));
}

/* Calculate one RAY and check direction of the walls */
void	calc_ray(t_player *player, float start_x, float *ray_x, float *ray_y)
{
	float	prev_x;
	float	prev_y;

	prev_x = 0;
	prev_y = 0;
	*ray_x = player->x;
	*ray_y = player->y;
	while (!touch(*ray_x, *ray_y, player->game))
	{
		prev_x = *ray_x;
		prev_y = *ray_y;
		*ray_x += cos(start_x);
		*ray_y += sin(start_x);
	}
	player->hit_x = prev_x;
	player->hit_y = prev_y;
	if (((int)prev_x / BLOCK != (int)*ray_x / BLOCK))
	{
		if (((int)(*ray_y) % BLOCK != 0 && (int)(*ray_y) % BLOCK != BLOCK - 1))
			player->hit_dir = is_west_east(start_x);
	}
	else if ((((int)prev_y / BLOCK != (int)*ray_y / BLOCK))
		&& ((int)(*ray_x) % BLOCK != 0))
		player->hit_dir = is_nord_sud(start_x);
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
