#include "level_internal.h"
#include <stdlib.h>
#include <string.h>

/* Bloc de lignes non vides à partir de first ; ensuite, seulement du vide */
static bool	map_extent(const t_lines *lines, int first, int *last,
		t_level_error *err)
{
	int	i;

	i = first;
	while (i < lines->n && !is_blank(&lines->v[i]))
		i++;
	*last = i;
	while (i < lines->n && is_blank(&lines->v[i]))
		i++;
	if (i < lines->n)
		return (level_fail(err, lines->v[i].num,
				"content after the map (the map must be the last element, "
				"without empty lines inside)"));
	return (true);
}

/* Un 'M' ou un 'X' : la case devient du sol, la position est gardée */
static bool	add_point(t_level *lv, const t_line *l, int x, int y,
		t_level_error *err)
{
	t_level_enemy	**v;
	int				*n;
	int				max;

	v = &lv->enemies;
	n = &lv->n_enemies;
	max = LEVEL_MAX_ENEMIES;
	if (l->s[x] == 'X')
	{
		v = &lv->spawns;
		n = &lv->n_spawns;
		max = LEVEL_MAX_SPAWNS;
	}
	if (*n == max && l->s[x] == 'X')
		return (level_fail(err, l->num, "more than %d spawn points", max));
	if (*n == max)
		return (level_fail(err, l->num, "more than %d mutants", max));
	if (!*v)
	{
		*v = malloc(sizeof(t_level_enemy) * max);
		if (!*v)
			return (level_fail(err, 0, "out of memory"));
	}
	(*v)[(*n)++] = (t_level_enemy){x, y, l->num};
	lv->cells[y * lv->w + x] = '0';
	return (true);
}

static bool	put_cell(t_level *lv, const t_line *l, int x, int y,
		t_level_error *err)
{
	char	c;

	c = l->s[x];
	if (is_space(c))
		return (true);
	if (c == 'M' || c == 'X')
		return (add_point(lv, l, x, y, err));
	if (c == '0' || c == '1')
	{
		lv->cells[y * lv->w + x] = c;
		return (true);
	}
	if (c != 'N' && c != 'S' && c != 'E' && c != 'W')
		return (level_fail(err, l->num, "unexpected character '%c' in the map",
				c));
	if (lv->spawn_dir)
		return (level_fail(err, l->num, "more than one starting position"));
	lv->spawn_dir = c;
	lv->spawn_x = x;
	lv->spawn_y = y;
	lv->cells[y * lv->w + x] = '0';
	return (true);
}

static bool	push(int *stack, int *top, char *seen, int i)
{
	if (seen[i])
		return (false);
	seen[i] = 1;
	stack[(*top)++] = i;
	return (true);
}

/*
 * Tout ce qu'on atteint depuis le départ sans traverser de mur, y compris en
 * diagonale, doit rester dans la grille. Pile explicite, pas de récursion.
 */
static bool	is_closed(const t_level *lv, int *stack, char *seen)
{
	int	top;
	int	i;
	int	d;
	int	x;
	int	y;

	top = 0;
	push(stack, &top, seen, lv->spawn_y * lv->w + lv->spawn_x);
	while (top > 0)
	{
		i = stack[--top];
		d = -1;
		while (++d < 9)
		{
			x = i % lv->w + d % 3 - 1;
			y = i / lv->w + d / 3 - 1;
			if (x < 0 || y < 0 || x >= lv->w || y >= lv->h)
				return (false);
			if (lv->cells[y * lv->w + x] != '1')
				push(stack, &top, seen, y * lv->w + x);
		}
	}
	return (true);
}

/* Chaque mutant et chaque point d'apparition doit être atteignable */
static bool	check_points(const t_level *lv, const char *seen,
		t_level_error *err)
{
	int	i;

	i = -1;
	while (++i < lv->n_enemies)
		if (!seen[lv->enemies[i].y * lv->w + lv->enemies[i].x])
			return (level_fail(err, lv->enemies[i].line,
					"mutant outside the area the player can reach"));
	i = -1;
	while (++i < lv->n_spawns)
		if (!seen[lv->spawns[i].y * lv->w + lv->spawns[i].x])
			return (level_fail(err, lv->spawns[i].line,
					"spawn point outside the area the player can reach"));
	return (true);
}

static bool	check_closed(const t_level *lv, int spawn_line, t_level_error *err)
{
	int		*stack;
	char	*seen;
	bool	ok;

	stack = malloc(sizeof(int) * (size_t)lv->w * lv->h);
	seen = calloc((size_t)lv->w * lv->h, 1);
	if (!stack || !seen)
	{
		free(stack);
		free(seen);
		return (level_fail(err, 0, "out of memory"));
	}
	ok = is_closed(lv, stack, seen);
	free(stack);
	if (!ok)
		level_fail(err, spawn_line,
			"the map is not closed: the player can walk off the edge");
	else
		ok = check_points(lv, seen, err);
	free(seen);
	return (ok);
}

bool	parse_map(const t_lines *lines, int first, t_level *lv,
		t_level_error *err)
{
	int	last;
	int	y;
	int	x;

	if (!map_extent(lines, first, &last, err))
		return (false);
	lv->h = last - first;
	lv->w = 0;
	y = first - 1;
	while (++y < last)
	{
		if (line_id(&lines->v[y]) >= 0)
			return (level_fail(err, lines->v[y].num, "identifier after the "
					"map (the map must be the last element)"));
		if (lines->v[y].len > lv->w)
			lv->w = lines->v[y].len;
	}
	if (lv->w > LEVEL_MAX_SIDE || lv->h > LEVEL_MAX_SIDE)
		return (level_fail(err, lines->v[first].num,
				"the map is %d x %d, the limit is %d x %d", lv->w, lv->h,
				LEVEL_MAX_SIDE, LEVEL_MAX_SIDE));
	lv->cells = malloc((size_t)lv->w * lv->h);
	if (!lv->cells)
		return (level_fail(err, 0, "out of memory"));
	memset(lv->cells, ' ', (size_t)lv->w * lv->h);
	y = -1;
	while (++y < lv->h)
	{
		x = -1;
		while (++x < lines->v[first + y].len)
			if (!put_cell(lv, &lines->v[first + y], x, y, err))
				return (false);
	}
	if (!lv->spawn_dir)
		return (level_fail(err, lines->v[first].num,
				"no starting position (N, S, E or W) in the map"));
	return (check_closed(lv, lines->v[first + lv->spawn_y].num, err));
}
