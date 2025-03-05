/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:30:33 by drongier          #+#    #+#             */
/*   Updated: 2025/03/05 18:45:21 by drongier         ###   ########.fr       */
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
    int		hit_dir = -1;

	*ray_x = player->x;
	*ray_y = player->y;
	while (!touch(*ray_x, *ray_y, player->game))
	{
		prev_x = *ray_x;
		prev_y = *ray_y;
		*ray_x += cos(start_x);
		*ray_y += sin(start_x);
	}
   // Détermination de la face touchée
   if ((int)prev_x / BLOCK != (int)*ray_x / BLOCK) // Changement en X -> Mur vertical (Est/Ouest)
   {
	   if (cos(start_x))
		   hit_dir = WEST;
	   else
			hit_dir = EAST;
	}
	else if ((int)prev_y / BLOCK != (int)*ray_y / BLOCK) // Changement en Y -> Mur horizontal (Nord/Sud)
	{
	   if (sin(start_x))
		   hit_dir = NORD;
	   else
		   hit_dir = SUD;
   }
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