#ifndef RASTER_H
# define RASTER_H

# include <stdint.h>
# include "motion.h"

# define RASTER_MAX_POINTS 512
# define ALPHA_OPAQUE 256

/* Zone de dessin : pixels hors de [x0, x1) x [y0, y1) ignorés */
typedef struct s_canvas
{
	uint32_t	*px;
	int			stride;
	int			x0;
	int			y0;
	int			x1;
	int			y1;
}	t_canvas;

/* alpha de 0 (dst inchangé) à ALPHA_OPAQUE (src) */
uint32_t	blend(uint32_t dst, uint32_t src, int alpha);
void		fill_span(t_canvas *c, int y, int from, int to, uint32_t color,
				int alpha);
/* Polygone quelconque (règle pair-impair), échantillonné au centre des pixels */
void		fill_polygon(t_canvas *c, const t_vec2 *pts, int n, uint32_t color,
				int alpha);

#endif
