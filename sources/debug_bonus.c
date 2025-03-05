/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:17:47 by drongier          #+#    #+#             */
/*   Updated: 2025/03/05 18:38:55 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	draw_square_full(int x, int y, int size, int color, t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			put_pixel(x + i, y + j, color, game);
			j++;
		}
		i++;
	}
}

//[BONUS] MINI MAP
void	draw_map(t_game *game, int offset_x, int offset_y)
{
	int		x;
	int		y;
	char	**map;
	int		color;

	map = game->map;
	color = 0x000000;
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == '1')
				draw_square_full(offset_x + x * BLOCK / 4, offset_y + y * BLOCK / 4, BLOCK / 4, color, game);
			x++;
		}
		y++;
	}
}

//[BONUS] Size & position minimap/player on the mini map
void	draw_mini_map(t_game *game)
{
	int	square_size;
	int	bottom_right_x;
	int	bottom_right_y;
	int	player_x;
	int	player_y;

	if (game->map_width > game->map_height)
		square_size = game->map_width * BLOCK / 4;
	else
		square_size = game->map_height * BLOCK / 4;
	bottom_right_x = WIDTH - square_size;
	bottom_right_y = HEIGHT - square_size;
	player_x = bottom_right_x + (game->player.x / BLOCK) * (BLOCK / 4);
	player_y = bottom_right_y + (game->player.y / BLOCK) * (BLOCK / 4);
	draw_map(game, bottom_right_x, bottom_right_y);
	draw_square_full(player_x, player_y, 5, 0x00FF00, game);
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