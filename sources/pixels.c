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

void	draw_sprite_column(uint32_t *dst, const t_wall_span *s,
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
	while (y++ < end)
	{
		if (!(col[q] & TEX_TRANSPARENT))
			*dst = col[q];
		dst += s->stride;
		q += tex_h / s->height;
		r += tex_h % s->height;
		if (r >= s->height)
		{
			r -= s->height;
			q++;
		}
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

/* Colonnes [*from, *to) de la ligne qui contiennent des pixels opaques */
static void	opaque_span(const uint32_t *row, int w, int *from, int *to)
{
	*from = 0;
	while (*from < w && (row[*from] & TEX_TRANSPARENT))
		(*from)++;
	*to = w;
	while (*to > *from && (row[*to - 1] & TEX_TRANSPARENT))
		(*to)--;
}

/* Chaque ligne source n'est parcourue que sur sa partie opaque */
void	blit_sprite(uint32_t *fb, int fb_w, int fb_h, const t_texture *t,
			t_blit at)
{
	const uint32_t	*src;
	int				span[2];
	int				x;
	int				end;
	int				y;

	y = at.y;
	if (y < 0)
		y = 0;
	while (y < at.y + t->height * at.scale && y < fb_h)
	{
		src = (const uint32_t *)t->data + (long)((y - at.y) / at.scale)
			* t->width;
		opaque_span(src, t->width, &span[0], &span[1]);
		x = at.x + span[0] * at.scale;
		if (x < 0)
			x = 0;
		end = at.x + span[1] * at.scale;
		if (end > fb_w)
			end = fb_w;
		while (x < end)
		{
			if (!(src[(x - at.x) / at.scale] & TEX_TRANSPARENT))
				fb[(long)y * fb_w + x] = src[(x - at.x) / at.scale];
			x++;
		}
		y++;
	}
}
