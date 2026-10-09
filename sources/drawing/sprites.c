#include "../../includes/cub3d.h"

/* Une case de 64 x 64 de la planche, de la taille d'un bloc à l'écran */
static void	draw_one(t_game *g, const t_proj *p, t_cell c)
{
	t_wall_span	span;
	const t_texture	*tex;
	int			left;
	int			x;
	int			end;

	tex = &g->mutant_tex;
	left = (int)(p->screen_x - p->size / 2.0f);
	span = (t_wall_span){(HEIGHT - p->size) / 2, p->size, HEIGHT, WIDTH};
	x = left;
	if (x < 0)
		x = 0;
	end = left + p->size;
	if (end > WIDTH)
		end = WIDTH;
	while (x < end)
	{
		if (p->depth < g->zbuf[x])
			draw_sprite_column(g->fb + x, &span, tex->columns
				+ (long)(c.col * 65 + 1 + (long)(x - left) * 64 / p->size)
				* tex->height + c.row * 65 + 1, 64);
		x++;
	}
}

/* Du plus loin au plus proche ; le z-buffer cache ce qui est derrière un mur */
void	draw_enemies(t_game *g, const t_camera *cam)
{
	int		order[LEVEL_MAX_ENEMIES];
	t_proj	proj[LEVEL_MAX_ENEMIES];
	int		n;
	int		i;
	int		j;
	t_vec2	viewer;

	viewer = (t_vec2){g->player.x / BLOCK, g->player.y / BLOCK};
	n = 0;
	i = -1;
	while (++i < g->horde.n)
		if (camera_project(cam, g->horde.v[i].pos, WIDTH, &proj[i])
			&& proj[i].size > 0)
			order[n++] = i;
	i = 0;
	while (++i < n)
	{
		j = i;
		while (j > 0 && proj[order[j - 1]].depth < proj[order[j]].depth)
		{
			order[j] ^= order[j - 1];
			order[j - 1] ^= order[j];
			order[j] ^= order[j - 1];
			j--;
		}
	}
	i = -1;
	while (++i < n)
		draw_one(g, &proj[order[i]], enemy_cell(&g->horde.v[order[i]],
				viewer));
}
