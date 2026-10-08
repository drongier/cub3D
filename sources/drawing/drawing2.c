/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:12:52 by drongier          #+#    #+#             */
/*   Updated: 2025/05/09 18:20:18 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:12:52 by drongier          #+#    #+#             */
/*   Updated: 2025/04/26 15:15:25 by mekundur         ###   ########.fr       */
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

/* Seule la partie visible du mur est parcourue */
void	draw_wall(int i, int start_y, int height, const t_hit *hit,
		t_game *game)
{
	t_texture	*texture;
	int			tex_x;
	int			y;
	int			end;

	texture = &game->textures[hit->face];
	tex_x = (int)(hit->wall_x * texture->width);
	y = start_y;
	if (y < 0)
		y = 0;
	end = start_y + height;
	if (end > HEIGHT)
		end = HEIGHT;
	while (y < end)
	{
		put_pixel(i, y, get_texture_color(texture, tex_x,
				(int)((long)(y - start_y) * texture->height / height)), game);
		y++;
	}
}

void	draw_ground(int i, int start_y, t_game *game)
{
	int	l;

	l = start_y;
	while (l < HEIGHT)
		put_pixel(i, l++, game->map->floor, game);
}
