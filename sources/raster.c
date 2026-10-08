#include "../includes/raster.h"
#include <math.h>

uint32_t	blend(uint32_t dst, uint32_t src, int alpha)
{
	uint32_t	rb;
	uint32_t	g;
	uint32_t	a;

	if (alpha >= ALPHA_OPAQUE)
		return (src & 0xFFFFFF);
	if (alpha <= 0)
		return (dst & 0xFFFFFF);
	a = (uint32_t)alpha;
	rb = ((src & 0xFF00FF) * a + (dst & 0xFF00FF) * (256 - a)) >> 8;
	g = ((src & 0x00FF00) * a + (dst & 0x00FF00) * (256 - a)) >> 8;
	return ((rb & 0xFF00FF) | (g & 0x00FF00));
}

/* Pixels [from, to) de la ligne y */
void	fill_span(t_canvas *c, int y, int from, int to, uint32_t color,
		int alpha)
{
	uint32_t	*row;

	if (y < c->y0 || y >= c->y1)
		return ;
	if (from < c->x0)
		from = c->x0;
	if (to > c->x1)
		to = c->x1;
	row = c->px + (long)y * c->stride;
	while (from < to)
	{
		row[from] = blend(row[from], color, alpha);
		from++;
	}
}

static void	sort_floats(float *v, int n)
{
	int		i;
	int		j;
	float	key;

	i = 1;
	while (i < n)
	{
		key = v[i];
		j = i - 1;
		while (j >= 0 && v[j] > key)
		{
			v[j + 1] = v[j];
			j--;
		}
		v[j + 1] = key;
		i++;
	}
}

/* Abscisses où les arêtes coupent la ligne horizontale yc */
static int	crossings(const t_vec2 *pts, int n, float yc, float *xs)
{
	int		i;
	int		k;
	t_vec2	a;
	t_vec2	b;

	k = 0;
	i = 0;
	while (i < n)
	{
		a = pts[i];
		b = pts[(i + 1) % n];
		if ((a.y <= yc && yc < b.y) || (b.y <= yc && yc < a.y))
			xs[k++] = a.x + (yc - a.y) * (b.x - a.x) / (b.y - a.y);
		i++;
	}
	return (k);
}

/*
 * Un pixel est rempli si son centre est dans le polygone : deux polygones
 * qui partagent une arête ne remplissent jamais deux fois le même pixel.
 */
void	fill_polygon(t_canvas *c, const t_vec2 *pts, int n, uint32_t color,
		int alpha)
{
	float	xs[RASTER_MAX_POINTS];
	int		y;
	int		k;
	int		i;

	if (n < 3 || n > RASTER_MAX_POINTS)
		return ;
	y = c->y0;
	while (y < c->y1)
	{
		k = crossings(pts, n, y + 0.5f, xs);
		sort_floats(xs, k);
		i = 0;
		while (i + 1 < k)
		{
			if (xs[i + 1] > c->x0 - 1 && xs[i] < c->x1 + 1)
				fill_span(c, y, (int)ceilf(fmaxf(xs[i], c->x0 - 1) - 0.5f),
					(int)ceilf(fminf(xs[i + 1], c->x1 + 1) - 0.5f),
					color, alpha);
			i += 2;
		}
		y++;
	}
}
