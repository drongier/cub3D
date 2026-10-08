#ifndef PIXELS_H
# define PIXELS_H

# include <stdbool.h>
# include <stdint.h>
# include "texture.h"

/* Lignes [y0, y1) d'un buffer de largeur stride, d'une seule couleur */
void	fill_rows(uint32_t *fb, int stride, int y0, int y1, uint32_t color);

/*
 * Colonne de mur : dst pointe sur la colonne x de la ligne 0. Le mur occupe
 * [start_y, start_y + height) à l'écran, seules les lignes de [0, screen_h)
 * sont écrites. Le texel de la ligne y est col[(y - start_y) * tex_h / height],
 * calculé sans division par pixel.
 */
typedef struct s_wall_span
{
	int	start_y;
	int	height;
	int	screen_h;
	int	stride;
}	t_wall_span;

void	draw_tex_column(uint32_t *dst, const t_wall_span *s,
			const uint32_t *col, int tex_h);

/* Copie transposée de la texture : la colonne x est contiguë */
bool	texture_build_columns(t_texture *t);
const uint32_t	*texture_column(const t_texture *t, int x);

#endif
