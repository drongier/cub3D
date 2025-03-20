/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:17:47 by drongier          #+#    #+#             */
/*   Updated: 2025/03/20 18:52:19 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	ray_tracing(t_game *game, float angle, float *x, float *y, t_minimap *mini)
{
	float	cos_a;
	float	sin_a;

	cos_a = cos(angle);
	sin_a = sin(angle);
	while (!touch(*x, *y, game))
	{
		put_pixel_minimap(game, *x, *y, mini);
		calcu_ray(x, y, cos_a, sin_a);
	}
}

void	draw_ray(t_game *game, float angle, t_minimap *mini)
{
	float	ray_x;
	float	ray_y;

	ray_x = game->player.x;
	ray_y = game->player.y;
	ray_tracing(game, angle, &ray_x, &ray_y, mini);
}

void	draw_square(int x, int y, int size, int color, t_game *game)
{
	int	i;
	int	j;

	i = -1;
	while (++i < size)
	{
		j = -1;
		while (++j < size)
			put_pixel(x + i, y + j, color, game);
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
					BLOCK / MM_SIZE, 0x000000, game);
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
	draw_square(x, y, 5, 0x00FF00, game);
}

void	draw_mini_map(t_game *game)
{
	t_minimap	mini;
	float		angle;
	float		step;
	int			i;

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
		draw_ray(game, angle, &mini);
		angle += step;
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
