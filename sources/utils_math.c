/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:30:33 by drongier          #+#    #+#             */
/*   Updated: 2025/03/07 15:02:44 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// distance calculation functions
float	distance(float x, float y)
{
	return (sqrt(x * x + y * y));
}

//Removing distortion + fisheyes effect 
float	fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
{
	float	delta_x;
	float	delta_y;
	float	angle;
	float	fix_dist;

	delta_x = x2 - x1;
	delta_y = y2 - y1;
	angle = atan2(delta_y, delta_x) - game->player.angle;
	fix_dist = distance(delta_x, delta_y) * cos(angle);
	return (fix_dist);
}

int	calc_ray(t_player *player, float start_x, float *ray_x, float *ray_y)
{
	float	prev_x;
	float	prev_y;
	int		hit_dir;

	*ray_x = player->x;
	*ray_y = player->y;
	while (!touch(*ray_x, *ray_y, player->game))
	{
		prev_x = *ray_x;
		prev_y = *ray_y;
		*ray_x += cos(start_x);
		*ray_y += sin(start_x);
	}
	if ((int)prev_x / BLOCK != (int)*ray_x / BLOCK)
		hit_dir = is_west_east(start_x);
	else if ((int)prev_y / BLOCK != (int)*ray_y / BLOCK)
		hit_dir = is_nord_sud(start_x);
	return (hit_dir);
}

float	calculate_height(t_player *player, float ray_x, float ray_y)
{
	float	dist;
	float	height;

	dist = fixed_dist(player->x, player->y, ray_x, ray_y, player->game);
	height = (BLOCK / dist) * (WIDTH / 2);
	return (height);
}