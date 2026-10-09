#include "../../includes/enemy.h"
#include <math.h>

static int	cell_of(const t_grid *g, t_vec2 p)
{
	int	x;
	int	y;

	x = (int)floorf(p.x);
	y = (int)floorf(p.y);
	if (x < 0 || y < 0 || x >= g->w || y >= g->h)
		return (-1);
	return (y * g->w + x);
}

static void	flow_push(t_horde *h, const t_grid *g, int *tail, int from, int to)
{
	if (g->cells[to] != CELL_FLOOR || h->flow[to] >= 0)
		return ;
	h->flow[to] = h->flow[from] + 1;
	h->queue[(*tail)++] = to;
}

/* Parcours en largeur depuis la case du joueur, seulement s'il en a changé */
void	flow_update(t_horde *h, const t_grid *g, t_vec2 player)
{
	int	start;
	int	head;
	int	tail;
	int	c;

	start = cell_of(g, player);
	if (start < 0 || start == h->flow_cell)
		return ;
	h->flow_cell = start;
	c = -1;
	while (++c < g->w * g->h)
		h->flow[c] = -1;
	h->flow[start] = 0;
	h->queue[0] = start;
	head = 0;
	tail = 1;
	while (head < tail)
	{
		c = h->queue[head++];
		if (c % g->w > 0)
			flow_push(h, g, &tail, c, c - 1);
		if (c % g->w < g->w - 1)
			flow_push(h, g, &tail, c, c + 1);
		if (c / g->w > 0)
			flow_push(h, g, &tail, c, c - g->w);
		if (c / g->w < g->h - 1)
			flow_push(h, g, &tail, c, c + g->w);
	}
}

bool	clear_line(const t_grid *g, t_vec2 a, t_vec2 b)
{
	t_vec2	d;
	float	len;

	d = (t_vec2){b.x - a.x, b.y - a.y};
	len = hypotf(d.x, d.y);
	if (len < 1e-4f)
		return (true);
	return (cast_ray(g, a, (t_vec2){d.x / len, d.y / len}).dist >= len);
}

bool	horde_blocks(const t_horde *h, t_vec2 p, float radius)
{
	int	i;

	i = -1;
	while (++i < h->n)
		if (h->v[i].state != EN_DYING && h->v[i].state != EN_DEAD
			&& hypotf(h->v[i].pos.x - p.x, h->v[i].pos.y - p.y)
			< radius + ENEMY_RADIUS)
			return (true);
	return (false);
}

/* Avancer de p vers q ferait-il heurter un autre ennemi vivant ? */
static bool	crowded(const t_horde *h, int i, t_vec2 p, t_vec2 q)
{
	const t_enemy	*o;
	int				j;

	j = -1;
	while (++j < h->n)
	{
		o = &h->v[j];
		if (j == i || o->state == EN_DYING || o->state == EN_DEAD)
			continue ;
		if (hypotf(q.x - o->pos.x, q.y - o->pos.y) < 2 * ENEMY_RADIUS
			&& hypotf(q.x - o->pos.x, q.y - o->pos.y)
			< hypotf(p.x - o->pos.x, p.y - o->pos.y))
			return (true);
	}
	return (false);
}

/* Un pas vers t ; false s'il rentre dans le joueur, un mur ou un ennemi */
static bool	try_step(t_horde *h, int i, const t_grid *g, t_vec2 t,
		t_vec2 player, double dt)
{
	t_enemy	*e;
	t_vec2	d;
	t_vec2	q;
	float	len;
	float	stepl;

	e = &h->v[i];
	d = (t_vec2){t.x - e->pos.x, t.y - e->pos.y};
	len = hypotf(d.x, d.y);
	if (len < 1e-4f)
		return (false);
	stepl = fminf(ENEMY_SPEED * (float)dt, len);
	q = (t_vec2){e->pos.x + d.x / len * stepl, e->pos.y + d.y / len * stepl};
	if (hypotf(player.x - q.x, player.y - q.y) < ENEMY_KEEP_AWAY
		|| crowded(h, i, e->pos, q)
		|| grid_at(g, (int)floorf(q.x), (int)floorf(q.y)) == CELL_WALL)
		return (false);
	e->facing = atan2f(d.y, d.x);
	e->pos = q;
	e->moving = true;
	e->walk_t += dt;
	return (true);
}

/* Case occupée par un autre ennemi vivant */
static bool	occupied(const t_horde *h, const t_grid *g, int i, int cell)
{
	int	j;

	j = -1;
	while (++j < h->n)
		if (j != i && h->v[j].state != EN_DYING && h->v[j].state != EN_DEAD
			&& cell_of(g, h->v[j].pos) == cell)
			return (true);
	return (false);
}

/* Ajoute la case n en gardant la liste triée par distance au joueur */
static void	insert(t_vec2 out[4], int *m, t_vec2 c, t_vec2 player)
{
	int	k;

	k = *m;
	while (k > 0 && hypotf(out[k - 1].x - player.x, out[k - 1].y - player.y)
		> hypotf(c.x - player.x, c.y - player.y))
	{
		out[k] = out[k - 1];
		k--;
	}
	out[k] = c;
	(*m)++;
}

/*
 * Cases voisines libres, d'abord celles qui rapprochent du joueur, puis
 * celles à la même distance, puis celles qui éloignent d'une case ; à
 * distance égale, la plus proche à vol d'oiseau. Si le chemin est occupé
 * par un autre ennemi, il contourne au lieu d'attendre.
 */
static int	candidates(const t_horde *h, const t_grid *g, int i,
		t_vec2 out[4])
{
	static const int	dx[4] = {1, -1, 0, 0};
	static const int	dy[4] = {0, 0, 1, -1};
	t_vec2				pos;
	int					pass;
	int					k;
	int					n;
	int					m;

	pos = h->v[i].pos;
	m = 0;
	pass = -1;
	while (++pass < 3 && m == 0)
	{
		k = -1;
		while (++k < 4)
		{
			n = cell_of(g, (t_vec2){pos.x + dx[k], pos.y + dy[k]});
			if (n >= 0 && h->flow[n] >= 0 && !occupied(h, g, i, n)
				&& h->flow[n] - h->flow[cell_of(g, pos)] == pass - 1)
				insert(out, &m, (t_vec2){n % g->w + 0.5f, n / g->w + 0.5f},
					h->player);
		}
	}
	return (m);
}

/*
 * Suit le champ de distances case par case ; dans la case voisine du joueur,
 * va droit sur lui et s'arrête à ENEMY_KEEP_AWAY.
 */
void	enemy_move(t_horde *h, int i, const t_grid *g, t_vec2 player,
		double dt)
{
	t_enemy	*e;
	t_vec2	t[4];
	int		c;
	int		n;
	int		k;

	e = &h->v[i];
	c = cell_of(g, e->pos);
	if (c < 0 || h->flow[c] < 0)
		return ;
	if (h->flow[c] <= 1)
	{
		if (!try_step(h, i, g, player, player, dt))
			e->facing = atan2f(player.y - e->pos.y, player.x - e->pos.x);
		return ;
	}
	h->player = player;
	n = candidates(h, g, i, t);
	k = -1;
	while (++k < n)
		if (try_step(h, i, g, t[k], player, dt))
			return ;
}
