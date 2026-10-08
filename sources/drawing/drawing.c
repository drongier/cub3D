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

/* Une colonne : plafond, mur texturé, sol */
static void	draw_column(t_game *game, const t_camera *cam, int i)
{
	t_hit	hit;
	int		height;
	int		start_y;

	hit = cast_ray(&game->grid, cam->pos, camera_ray(cam, i, WIDTH));
	game->hits[i] = (t_vec2){hit.point.x * BLOCK, hit.point.y * BLOCK};
	height = wall_height(cam, hit.dist);
	start_y = (HEIGHT - height) / 2;
	draw_ceiling(i, start_y, game);
	draw_wall(i, start_y, height, &hit, game);
	draw_ground(i, start_y + height, game);
}

/* GRAPHIC ENGINE */
int	draw_loop(t_game *game)
{
	t_camera	cam;
	int			i;

	cam = camera_make((t_vec2){game->player.x / BLOCK,
			game->player.y / BLOCK}, game->player.angle, WIDTH);
	i = 0;
	while (i < WIDTH)
		draw_column(game, &cam, i++);
	if (BONUS == 1)
	{
		draw_minimap(game);
		draw_scope(game);
	}
	return (0);
}
