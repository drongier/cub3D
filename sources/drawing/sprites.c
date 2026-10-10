#include "../../includes/cub3d.h"

/* Mutants (y compris les places des vagues) et trousses */
#define MAX_SPRITES (LEVEL_MAX_ENEMIES + HORDE_WAVE_SLOTS + LEVEL_MAX_ITEMS)

/*
 * Un sprite à l'écran : une case de 64 x 64 de tex, dont le coin est en
 * (u, v) dans la texture, de la taille d'un bloc
 */
typedef struct s_sprite
{
	const t_camera	*cam;
	const t_texture	*tex;
	int				u;
	int				v;
	t_proj			p;
}	t_sprite;

static void	draw_one(t_game *g, const t_sprite *s)
{
	t_wall_span	span;
	int			left;
	int			x;
	int			end;

	left = (int)(s->p.screen_x - s->p.size / 2.0f);
	span = (t_wall_span){(HEIGHT - s->p.size) / 2, s->p.size, HEIGHT, WIDTH};
	x = left;
	if (x < 0)
		x = 0;
	end = left + s->p.size;
	if (end > WIDTH)
		end = WIDTH;
	while (x < end)
	{
		if (s->p.depth < g->zbuf[x])
			draw_sprite_column(g->fb + x, &span, s->tex->columns
				+ (long)(s->u + (long)(x - left) * 64 / s->p.size)
				* s->tex->height + s->v, 64);
		x++;
	}
}

/* Ajoute le sprite s placé en pos (en cases) s'il est devant la caméra */
static void	add(t_sprite *v, int *n, t_vec2 pos, t_sprite s)
{
	if (camera_project(s.cam, pos, WIDTH, &s.p) && s.p.size > 0)
		v[(*n)++] = s;
}

/* Tous les sprites devant la caméra */
static int	collect(t_game *g, const t_camera *cam, t_sprite *v)
{
	t_vec2	viewer;
	t_cell	c;
	int		n;
	int		i;

	viewer = (t_vec2){g->player.x / BLOCK, g->player.y / BLOCK};
	n = 0;
	i = -1;
	while (++i < g->horde.n && n < MAX_SPRITES)
	{
		c = enemy_cell(&g->horde.v[i], viewer);
		add(v, &n, g->horde.v[i].pos, (t_sprite){cam, &g->mutant_tex,
			c.col * 65 + 1, c.row * 65 + 1, {0}});
	}
	i = -1;
	while (++i < g->items.n && n < MAX_SPRITES)
		if (g->items.v[i].present)
			add(v, &n, g->items.v[i].pos, (t_sprite){cam, &g->medkit_tex, 0, 0,
				{0}});
	return (n);
}

/* Du plus loin au plus proche ; le z-buffer cache ce qui est derrière un mur */
void	draw_sprites(t_game *g, const t_camera *cam)
{
	static t_sprite	v[MAX_SPRITES];
	t_sprite		tmp;
	int				n;
	int				i;
	int				j;

	n = collect(g, cam, v);
	i = 0;
	while (++i < n)
	{
		tmp = v[i];
		j = i;
		while (j > 0 && v[j - 1].p.depth < tmp.p.depth)
		{
			v[j] = v[j - 1];
			j--;
		}
		v[j] = tmp;
	}
	i = -1;
	while (++i < n)
		draw_one(g, &v[i]);
}
