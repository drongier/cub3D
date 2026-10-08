#include "../includes/pixels.h"
#include <stdlib.h>

void	fill_rows(uint32_t *fb, int stride, int y0, int y1, uint32_t color)
{
	uint32_t	*p;
	uint32_t	*end;

	p = fb + (long)y0 * stride;
	end = fb + (long)y1 * stride;
	while (p < end)
		*p++ = color;
}

/*
 * Indice de texel q et reste r de (y - start_y) * tex_h / height, avancés
 * d'une ligne à l'autre : même résultat que la division, sans la division.
 */
void	draw_tex_column(uint32_t *dst, const t_wall_span *s,
			const uint32_t *col, int tex_h)
{
	long	q;
	long	r;
	int		y;
	int		end;

	y = s->start_y;
	if (y < 0)
		y = 0;
	end = s->start_y + s->height;
	if (end > s->screen_h)
		end = s->screen_h;
	q = (long)(y - s->start_y) * tex_h / s->height;
	r = (long)(y - s->start_y) * tex_h % s->height;
	dst += (long)y * s->stride;
	while (y < end)
	{
		*dst = col[q];
		dst += s->stride;
		q += tex_h / s->height;
		r += tex_h % s->height;
		if (r >= s->height)
		{
			r -= s->height;
			q++;
		}
		y++;
	}
}

bool	texture_build_columns(t_texture *t)
{
	const uint32_t	*px;
	int				x;
	int				y;

	px = (const uint32_t *)t->data;
	t->columns = malloc(sizeof(uint32_t) * (size_t)t->width * t->height);
	if (!t->columns)
		return (false);
	x = -1;
	while (++x < t->width)
	{
		y = -1;
		while (++y < t->height)
			t->columns[(long)x * t->height + y] = px[(long)y * t->width + x];
	}
	return (true);
}

const uint32_t	*texture_column(const t_texture *t, int x)
{
	if (x < 0)
		x = 0;
	if (x >= t->width)
		x = t->width - 1;
	return (t->columns + (long)x * t->height);
}
