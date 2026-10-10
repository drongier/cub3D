#ifndef BITMAP_FONT_H
# define BITMAP_FONT_H

# include <stdint.h>

/* Police 5 x 7 : majuscules sans accents, chiffres, espace et  / : ! . - + */
# define FONT_W 5
# define FONT_H 7
# define FONT_ADVANCE 6

typedef struct s_text
{
	int			x;
	int			y;
	int			scale;
	uint32_t	color;
}	t_text;

/* Largeur en pixels du texte une fois dessiné */
int		font_width(const char *s, int scale);
/* Les caractères inconnus sont dessinés comme des espaces */
void	font_draw(uint32_t *fb, int fb_w, int fb_h, t_text t, const char *s);

#endif
