#ifndef TEXTURE_H
# define TEXTURE_H

# include <stdbool.h>
# include <stdint.h>

/*
 * data pointe sur un uint32_t[width * height], pixels en 0x00RRGGBB.
 * columns : la même image transposée (colonne x contiguë), ou NULL.
 */
typedef struct s_texture
{
	char		*data;
	uint32_t	*columns;
	int			width;
	int			height;
	int			bpp;
	int			size_line;
}	t_texture;

bool	xpm_load(const char *path, t_texture *out);
void	texture_free(t_texture *t);

#endif
