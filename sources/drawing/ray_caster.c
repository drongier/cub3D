/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_caster.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:04:43 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/12 14:54:49 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// Calculate the exact hit position
void	ft_exact(t_player *player, t_ray *ray, float *ray_x, float *ray_y)
{
	if (ray->side == 0)
	{
		*ray_x = ray->map_x * BLOCK;
		if (ray->step_x < 0)
			*ray_x += BLOCK;
		*ray_y = player->y + (*ray_x - player->x) * ray->dir_y / ray->dir_x;
		if (ray->step_x > 0)
			player->hit_dir = EAST;
		else
			player->hit_dir = WEST;
	}
	else
	{
		*ray_y = ray->map_y * BLOCK;
		if (ray->step_y < 0)
			*ray_y += BLOCK;
		*ray_x = player->x + (*ray_y - player->y) * ray->dir_x / ray->dir_y;
		if (ray->step_y > 0)
			player->hit_dir = SUD;
		else
			player->hit_dir = NORD;
	}
}

// Perform DDA
void	ft_dda(t_player *player, t_ray *ray)
{
	bool	hit;

	hit = false;
	while (!hit)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (player->game->map->map[ray->map_y][ray->map_x] == '1')
			hit = true;
	}
}

// Calculate step and initial sideDist
void	ft_step(t_player *player, t_ray *ray)
{
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (player->x - ray->map_x * BLOCK) \
				* ray->delta_dist_x / BLOCK;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = ((ray->map_x + 1) * BLOCK - player->x) \
				* ray->delta_dist_x / BLOCK;
	}
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (player->y - ray->map_y * BLOCK) \
				* ray->delta_dist_y / BLOCK;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = ((ray->map_y + 1) * BLOCK - player->y) \
				* ray->delta_dist_y / BLOCK;
	}
}

void	calc_ray(t_player *player, float start_x, float *ray_x, float *ray_y)
{
	t_ray	ray;

	ray.angle = start_x;
	ray.dir_x = cos(ray.angle);
	ray.dir_y = sin(ray.angle);
	ray.map_x = (int)(player->x) / BLOCK;
	ray.map_y = (int)(player->y) / BLOCK;
	if (ray.dir_x == 0)
		ray.delta_dist_x = 3.4e38;
	else
		ray.delta_dist_x = fabsf(1 / ray.dir_x);
	if (ray.dir_y == 0)
		ray.delta_dist_y = 3.4e38;
	else
		ray.delta_dist_y = fabsf(1 / ray.dir_y);
	ft_step(player, &ray);
	ft_dda(player, &ray);
	ft_exact(player, &ray, ray_x, ray_y);
	player->hit_x = *ray_x;
	player->hit_y = *ray_y;
}
