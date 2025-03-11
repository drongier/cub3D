/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:12:52 by drongier          #+#    #+#             */
/*   Updated: 2025/03/11 13:50:32 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	draw_ceiling(int i, int start_y, t_game *game)
{
	int	j;

	j = 0;
	while (j < start_y)
		put_pixel(i, j++, 0x74583A, game);
}

void	draw_wall(int i, int *start_y, int end, t_game *game)
{
	int	color;

	if (game->player.hit_dir == NORD)
		color = 0xFF0000;
	else if (game->player.hit_dir == SUD)
		color = 0x00FF00;
	else if (game->player.hit_dir == WEST)
		color = 0x0000FF;
	else
		color = 0xFFFF00;

	while (*start_y < end)
		put_pixel(i, (*start_y)++, color, game);
	if (*start_y < 0)
		*start_y = 0;
}

void	draw_ground(int i, int start_y, t_game *game)
{
	int	l;

	l = start_y;
	while (l < HEIGHT)
		put_pixel(i, l++, 0x999999, game);
}
