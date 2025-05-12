/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:01:36 by drongier          #+#    #+#             */
/*   Updated: 2025/05/12 14:42:34 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	put_pixel(int x, int y, int color, t_game *game)
{
	int	index;

	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return ;
	index = y * game->size_line + x * game->bpp / 8;
	game->data[index] = color & 0xFF;
	game->data[index + 1] = (color >> 8) & 0xFF;
	game->data[index + 2] = (color >> 16) & 0xFF;
}

/* RAYCASTING FONCTION  
	-> Calculate ray and define direction of player
	-> Define height of wall to play with perspective
	-> Draw wall / Celling / floor */
void	draw_line(t_player *player, t_game *game, float start_x, int i)
{
	float	ray_x;
	float	ray_y;
	float	height;
	int		start_y;
	int		end;

	calc_ray(player, start_x, &ray_x, &ray_y);
	height = calculate_height(player, ray_x, ray_y);
	start_y = (HEIGHT - height) / 2;
	end = start_y + height;
	draw_ceiling(i, start_y, game);
	draw_wall(i, &start_y, end, game);
	draw_ground(i, start_y, game);
}

/* GRAPHIC ENGINE */
int	draw_loop(t_game *game)
{
	t_player	*player;
	float		fraction;
	float		start_x;
	int			i;

	player = &game->player;
	fraction = PI / 3 / WIDTH;
	start_x = player->angle - PI / 6;
	i = 0;
	move_player(player);
	while (i < WIDTH)
	{
		draw_line(player, game, start_x, i);
		start_x += fraction;
		i++;
	}
	if (BONUS == 1)
	{
		draw_mini_map(game);
		draw_scope(game);
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}
