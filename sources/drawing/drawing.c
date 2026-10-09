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

/* Une colonne de mur ; plafond et sol sont déjà remplis */
static void	draw_column(t_game *game, const t_camera *cam, int i)
{
	t_hit		hit;
	t_texture	*tex;
	t_wall_span	span;

	hit = cast_ray(&game->grid, cam->pos, camera_ray(cam, i, WIDTH));
	game->hits[i] = (t_vec2){hit.point.x * BLOCK, hit.point.y * BLOCK};
	span.height = wall_height(cam, hit.dist);
	if (span.height <= 0)
		return ;
	span.start_y = (HEIGHT - span.height) / 2;
	span.screen_h = HEIGHT;
	span.stride = WIDTH;
	tex = &game->textures[hit.face];
	draw_tex_column(game->fb + i, &span,
		texture_column(tex, (int)(hit.wall_x * tex->width)), tex->height);
}

/* Arme en bas au centre, décalée par le balancement */
void	draw_weapon(t_game *game)
{
	const t_texture	*tex;
	t_vec2			o;

	tex = &game->weapon_tex[weapon_frame(&game->weapon)];
	o = weapon_offset(&game->weapon);
	blit_sprite(game->fb, WIDTH, HEIGHT, tex, (t_blit){
		WIDTH / 2 - tex->width * WEAPON_SCALE / 2 + (int)o.x,
		HEIGHT - tex->height * WEAPON_SCALE + (int)o.y, WEAPON_SCALE});
}

/* GRAPHIC ENGINE */
int	draw_loop(t_game *game)
{
	t_camera	cam;
	int			i;

	fill_rows(game->fb, WIDTH, 0, HEIGHT / 2,
		game->level.ceiling);
	fill_rows(game->fb, WIDTH, HEIGHT / 2, HEIGHT,
		game->level.floor);
	cam = camera_make((t_vec2){game->player.x / BLOCK,
			game->player.y / BLOCK}, game->player.angle, WIDTH);
	i = 0;
	while (i < WIDTH)
		draw_column(game, &cam, i++);
	draw_weapon(game);
	if (BONUS == 1)
	{
		draw_minimap(game);
		draw_scope(game);
	}
	return (0);
}
