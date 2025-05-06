/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_map2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:50:15 by drongier          #+#    #+#             */
/*   Updated: 2025/05/06 18:23:15 by drongier         ###   ########.fr       */
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

int	check_cells(t_game *game, float next_x, float next_y)
{
	int	curr_x;
	int	curr_y;
	int	next_cell_x;
	int	next_cell_y;

	curr_x = get_cell_index(game->ray.x);
	curr_y = get_cell_index(game->ray.y);
	next_cell_x = get_cell_index(next_x);
	next_cell_y = get_cell_index(next_y);
	if (is_diagonal(curr_x, curr_y, next_cell_x, next_cell_y))
	{
		if (touch(game->ray.x, next_y, game) \
			|| touch(next_x, game->ray.y, game))
			return (1);
	}
	return (0);
}

int	check_colli(t_game *game)
{
	float	next_x;
	float	next_y;

	next_x = game->ray.x + game->ray.cos_a;
	next_y = game->ray.y + game->ray.sin_a;
	if (check_cells(game, next_x, next_y))
		return (1);
	game->ray.x = next_x;
	game->ray.y = next_y;
	return (0);
}

void	ray_tracing(t_game *game, float angle)
{
	t_minimap	*mini;

	game->ray.cos_a = cos(angle);
	game->ray.sin_a = sin(angle);
	mini = game->minimap;
	while (!touch(game->ray.x, game->ray.y, game))
	{
		put_pixel_minimap(game, game->ray.x, game->ray.y, mini);
		if (check_colli(game))
			break ;
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
