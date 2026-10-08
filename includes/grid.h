#ifndef GRID_H
# define GRID_H

# include <stdbool.h>
# include <stdint.h>

enum e_cell
{
	CELL_FLOOR,
	CELL_WALL,
	CELL_VOID
};

/* Map en tableau plat w x h ; hors de la grille, tout est mur */
typedef struct s_grid
{
	uint8_t	*cells;
	int		w;
	int		h;
}	t_grid;

/* rows : h lignes d'au moins w caractères, '1' pour un mur */
bool	grid_init(t_grid *g, char **rows, int w, int h);
void	grid_free(t_grid *g);
uint8_t	grid_at(const t_grid *g, int x, int y);

#endif
