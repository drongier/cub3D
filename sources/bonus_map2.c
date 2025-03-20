/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_map2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:50:15 by drongier          #+#    #+#             */
/*   Updated: 2025/03/20 18:52:16 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
