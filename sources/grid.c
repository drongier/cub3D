#include "../includes/grid.h"
#include <stdlib.h>

static void	push_if_floor(const t_grid *g, int *stack, int *top, int i)
{
	if (g->cells[i] == CELL_FLOOR)
		stack[(*top)++] = i;
}

/* Les cases vides reliées au bord de la grille sont hors de la map */
static void	flood_outside(t_grid *g, int *stack)
{
	int	top;
	int	i;

	top = 0;
	i = -1;
	while (++i < g->w * g->h)
		if (i % g->w == 0 || i % g->w == g->w - 1 || i / g->w == 0
			|| i / g->w == g->h - 1)
			push_if_floor(g, stack, &top, i);
	while (top > 0)
	{
		i = stack[--top];
		if (g->cells[i] != CELL_FLOOR)
			continue ;
		g->cells[i] = CELL_VOID;
		if (i % g->w > 0)
			push_if_floor(g, stack, &top, i - 1);
		if (i % g->w < g->w - 1)
			push_if_floor(g, stack, &top, i + 1);
		if (i / g->w > 0)
			push_if_floor(g, stack, &top, i - g->w);
		if (i / g->w < g->h - 1)
			push_if_floor(g, stack, &top, i + g->w);
	}
}

bool	grid_init(t_grid *g, char **rows, int w, int h)
{
	int	*stack;
	int	i;

	g->w = w;
	g->h = h;
	g->cells = malloc((size_t)w * h + 1);
	stack = malloc(sizeof(int) * ((size_t)w * h * 5 + 1));
	if (!g->cells || !stack)
	{
		free(stack);
		grid_free(g);
		return (false);
	}
	i = -1;
	while (++i < w * h)
	{
		g->cells[i] = CELL_FLOOR;
		if (rows[i / w][i % w] == '1')
			g->cells[i] = CELL_WALL;
	}
	flood_outside(g, stack);
	free(stack);
	return (true);
}

void	grid_free(t_grid *g)
{
	free(g->cells);
	g->cells = NULL;
}

uint8_t	grid_at(const t_grid *g, int x, int y)
{
	if (x < 0 || y < 0 || x >= g->w || y >= g->h)
		return (CELL_WALL);
	return (g->cells[y * g->w + x]);
}
