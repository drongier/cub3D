/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:17:47 by drongier          #+#    #+#             */
/*   Updated: 2025/03/21 11:22:01 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_ray(t_game *game, float angle)
{
	float	ray_x;
	float	ray_y;

	ray_x = game->player.x;
	ray_y = game->player.y;
	ray_tracing(game, angle, &ray_x, &ray_y);
}

void	draw_square(int x, int y, int size, t_game *game)
{
	int	i;
	int	j;

	i = -1;
	while (++i < size)
	{
		j = -1;
		while (++j < size)
			put_pixel(x + i, y + j, 0, game);
	}
}

void	draw_map(t_game *game, t_minimap *mini)
{
	int		x;
	int		y;
	char	**map;

	map = game->map->map;
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == '1')
				draw_square(mini->bottom_right_x + x * BLOCK / MM_SIZE,
					mini->bottom_right_y + y * BLOCK / MM_SIZE,
					BLOCK / MM_SIZE, game);
			x++;
		}
		y++;
	}
}

void	draw_player(t_game *game, t_minimap *mini)
{
	int	x;
	int	y;

	x = mini->bottom_right_x + (game->player.x / BLOCK) * (BLOCK / MM_SIZE);
	y = mini->bottom_right_y + (game->player.y / BLOCK) * (BLOCK / MM_SIZE);
	draw_square(x, y, 5, game);
}

void	draw_mini_map(t_game *game)
{
	t_minimap	mini;
	float		angle;
	float		step;
	int			i;

	game->minimap = &mini;
	if (game->map->col > game->map->row)
		mini.square_size = game->map->col * BLOCK / MM_SIZE;
	else
		mini.square_size = game->map->row * BLOCK / MM_SIZE;
	mini.bottom_right_x = WIDTH - mini.square_size;
	mini.bottom_right_y = HEIGHT - mini.square_size;
	draw_map(game, &mini);
	draw_player(game, &mini);
	step = PI / 3 / WIDTH;
	angle = game->player.angle - PI / 6;
	i = -1;
	while (++i < WIDTH)
	{
		draw_ray(game, angle);
		angle += step;
	}
}
