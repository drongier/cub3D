#include "../../includes/cub3d.h"

/*
 * Radar en bas à droite : joueur au centre, nord en haut, la carte défile.
 * Cases lues dans game->grid ; à chaque frame, fond, cône de
 * vision construit avec les impacts du rendu 3D, flèche du joueur, cadre.
 */

static uint8_t	cell_at(const t_grid *g, float wx, float wy)
{
	int	x;
	int	y;

	if (wx < 0 || wy < 0)
		return (CELL_VOID);
	x = (int)(wx / BLOCK);
	y = (int)(wy / BLOCK);
	if (x >= g->w || y >= g->h)
		return (CELL_VOID);
	return (g->cells[y * g->w + x]);
}

/* Fond : chaque pixel du radar prend la case du monde qu'il recouvre */
static void	draw_background(t_game *g, t_canvas *c)
{
	static const uint32_t	color[3] = {MM_FLOOR_RGB, MM_WALL_RGB, MM_VOID_RGB};
	static const int		alpha[3] = {MM_FLOOR_A, MM_WALL_A, MM_VOID_A};
	float					scale;
	float					wy;
	uint8_t					cell;
	int						xy[2];

	scale = (float)BLOCK / MM_CELL;
	xy[1] = c->y0 - 1;
	while (++xy[1] < c->y1)
	{
		wy = g->player.y + (xy[1] + 0.5f - (c->y0 + MM_SIZE / 2.0f)) * scale;
		xy[0] = c->x0 - 1;
		while (++xy[0] < c->x1)
		{
			cell = cell_at(&g->grid, g->player.x
					+ (xy[0] + 0.5f - (c->x0 + MM_SIZE / 2.0f)) * scale, wy);
			c->px[xy[1] * c->stride + xy[0]] = blend(
					c->px[xy[1] * c->stride + xy[0]], color[cell], alpha[cell]);
		}
	}
}

/* Éventail joueur -> impacts, un point toutes les MM_CONE_STEP colonnes */
static void	draw_cone(t_game *g, t_canvas *c, t_vec2 center)
{
	t_vec2	pts[WIDTH / MM_CONE_STEP + 3];
	float	k;
	int		n;
	int		col;

	k = (float)MM_CELL / BLOCK;
	pts[0] = center;
	n = 1;
	col = 0;
	while (col < WIDTH)
	{
		pts[n++] = (t_vec2){center.x + (g->hits[col].x - g->player.x) * k,
			center.y + (g->hits[col].y - g->player.y) * k};
		if (col == WIDTH - 1)
			break ;
		col += MM_CONE_STEP;
		if (col > WIDTH - 1)
			col = WIDTH - 1;
	}
	fill_polygon(c, pts, n, MM_CONE_RGB, MM_CONE_A);
}

static void	draw_arrow(t_game *g, t_canvas *c, t_vec2 o)
{
	t_vec2	d;
	t_vec2	pts[4];

	d = (t_vec2){cosf(g->player.angle), sinf(g->player.angle)};
	pts[0] = (t_vec2){o.x + d.x * 7, o.y + d.y * 7};
	pts[1] = (t_vec2){o.x - d.x * 5 - d.y * 5, o.y - d.y * 5 + d.x * 5};
	pts[2] = (t_vec2){o.x - d.x * 2, o.y - d.y * 2};
	pts[3] = (t_vec2){o.x - d.x * 5 + d.y * 5, o.y - d.y * 5 - d.x * 5};
	fill_polygon(c, pts, 4, MM_PLAYER_RGB, ALPHA_OPAQUE);
}

static void	draw_frame(t_canvas *c)
{
	int	y;

	fill_span(c, c->y0, c->x0, c->x1, MM_FRAME_RGB, MM_FRAME_A);
	fill_span(c, c->y1 - 1, c->x0, c->x1, MM_FRAME_RGB, MM_FRAME_A);
	y = c->y0 + 1;
	while (y < c->y1 - 1)
	{
		fill_span(c, y, c->x0, c->x0 + 1, MM_FRAME_RGB, MM_FRAME_A);
		fill_span(c, y, c->x1 - 1, c->x1, MM_FRAME_RGB, MM_FRAME_A);
		y++;
	}
}

void	draw_minimap(t_game *g)
{
	t_canvas	c;
	t_vec2		center;

	c = (t_canvas){g->fb, WIDTH, WIDTH - MM_MARGIN - MM_SIZE,
		HEIGHT - MM_MARGIN - MM_SIZE, WIDTH - MM_MARGIN, HEIGHT - MM_MARGIN};
	center = (t_vec2){c.x0 + MM_SIZE / 2.0f, c.y0 + MM_SIZE / 2.0f};
	draw_background(g, &c);
	draw_cone(g, &c, center);
	draw_arrow(g, &c, center);
	draw_frame(&c);
}

void	draw_scope(t_game *g)
{
	t_canvas	c;

	c = (t_canvas){g->fb, WIDTH, 0, 0, WIDTH, HEIGHT};
	fill_span(&c, HEIGHT / 2, WIDTH / 2 - 2, WIDTH / 2 + 3, 0, ALPHA_OPAQUE);
	fill_span(&c, HEIGHT / 2 - 2, WIDTH / 2, WIDTH / 2 + 1, 0, ALPHA_OPAQUE);
	fill_span(&c, HEIGHT / 2 - 1, WIDTH / 2, WIDTH / 2 + 1, 0, ALPHA_OPAQUE);
	fill_span(&c, HEIGHT / 2 + 1, WIDTH / 2, WIDTH / 2 + 1, 0, ALPHA_OPAQUE);
	fill_span(&c, HEIGHT / 2 + 2, WIDTH / 2, WIDTH / 2 + 1, 0, ALPHA_OPAQUE);
}
