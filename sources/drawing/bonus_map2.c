/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_map2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:50:15 by drongier          #+#    #+#             */
/*   Updated: 2025/03/21 11:27:44 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	put_pixel_minimap(t_game *game, float x, float y, t_minimap *mini)
{
	int	map_x;
	int	map_y;

	map_x = mini->bottom_right_x + (x / BLOCK) * (BLOCK / MM_SIZE);
	map_y = mini->bottom_right_y + (y / BLOCK) * (BLOCK / MM_SIZE);
	put_pixel(map_x, map_y, 0xAAAAAA, game);
}

void	calcu_ray(float *x, float *y, float cos_a, float sin_a)
{
	*x += cos_a;
	*y += sin_a;
}

void	ray_tracing(t_game *game, float angle, float *x, float *y)
{
	t_minimap	*mini;
	float		cos_a;
	float		sin_a;

	mini = game->minimap;
	cos_a = cos(angle);
	sin_a = sin(angle);
	while (!touch(*x, *y, game))
	{
		put_pixel_minimap(game, *x, *y, mini);
		calcu_ray(x, y, cos_a, sin_a);
	}
}

//[BONUS] Really optionnal -> Scope on map
void	draw_scope(t_game *game)
{
	int	x;
	int	y;
	int	line_length;
	int	center_x;
	int	center_y;

	line_length = 5;
	center_x = WIDTH / 2;
	center_y = HEIGHT / 2;
	x = center_x - line_length / 2;
	while (x <= center_x + line_length / 2)
		put_pixel(x++, center_y, 0x000000, game);
	y = center_y - line_length / 2;
	while (y <= center_y + line_length / 2)
		put_pixel(center_x, y++, 0x000000, game);
}
