/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:12:52 by drongier          #+#    #+#             */
/*   Updated: 2025/03/21 11:27:48 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_ceiling(int i, int start_y, t_game *game)
{
	int	j;

	j = 0;
	while (j < start_y)
		put_pixel(i, j++, game->map->ceiling, game);
}

int	get_texture_color(t_texture *texture, int tex_x, int tex_y)
{
	int	index;

	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= texture->width)
		tex_x = texture->width - 1;
	if (tex_y < 0)
		tex_y = 0;
	if (tex_y >= texture->height)
		tex_y = texture->height - 1;
	index = (tex_y * texture->size_line) + (tex_x * (texture->bpp / 8));
	return (*(int *)(texture->data + index));
}

void	select_texture(t_game *game, int *tex_x)
{
	t_texture	*texture;

	if (game->player.hit_dir == NORD)
		texture = &game->textures[0];
	else if (game->player.hit_dir == SUD)
		texture = &game->textures[1];
	else if (game->player.hit_dir == WEST)
		texture = &game->textures[2];
	else
		texture = &game->textures[3];
	if (game->player.hit_dir == NORD || game->player.hit_dir == SUD)
		*tex_x = (int)game->player.hit_x % BLOCK * texture->width / BLOCK;
	else
		*tex_x = (int)game->player.hit_y % BLOCK * texture->width / BLOCK;
}

void	draw_wall(int i, int *start_y, int end, t_game *game)
{
	int			tex_x;
	int			tex_y;
	int			wall_height;
	t_texture	*texture;

	wall_height = end - *start_y;
	select_texture(game, &tex_x);
	if (game->player.hit_dir == NORD)
		texture = &game->textures[0];
	else if (game->player.hit_dir == SUD)
		texture = &game->textures[1];
	else if (game->player.hit_dir == WEST)
		texture = &game->textures[2];
	else
		texture = &game->textures[3];
	while (*start_y < end)
	{
		tex_y = (*start_y - (HEIGHT / 2) + (wall_height / 2)) * texture->height
			/ wall_height;
		put_pixel(i, (*start_y)++, get_texture_color(texture, tex_x, tex_y),
			game);
	}
}

void	draw_ground(int i, int start_y, t_game *game)
{
	int	l;

	l = start_y;
	while (l < HEIGHT)
		put_pixel(i, l++, game->map->floor, game);
}
