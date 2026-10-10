#include "../../includes/enemy.h"
#include <math.h>

/* Point libre, à au moins min_steps cases de marche, hors de vue si hidden */
static bool	suits(const t_horde *h, const t_grid *g, t_level_enemy s,
		int min_steps, bool hidden)
{
	int		steps;
	t_vec2	p;

	steps = h->flow[s.y * g->w + s.x];
	p = (t_vec2){s.x + 0.5f, s.y + 0.5f};
	if (steps < min_steps)
		return (false);
	if (hidden && clear_line(g, p, h->player))
		return (false);
	return (!horde_blocks(h, p, ENEMY_RADIUS));
}

/* Un point au hasard parmi ceux qui conviennent ; -1 s'il n'y en a aucun */
static int	pick(t_horde *h, const t_grid *g, const t_level *lv, bool strict)
{
	int	count;
	int	k;
	int	i;
	int	min_steps;

	min_steps = SPAWN_FALLBACK_STEPS;
	if (strict)
		min_steps = SPAWN_MIN_STEPS;
	count = 0;
	i = -1;
	while (++i < lv->n_spawns)
		count += suits(h, g, lv->spawns[i], min_steps, strict);
	if (count == 0)
		return (-1);
	k = (int)(horde_rand(h) % (uint32_t)count);
	i = -1;
	while (++i < lv->n_spawns)
		if (suits(h, g, lv->spawns[i], min_steps, strict) && k-- == 0)
			return (i);
	return (-1);
}

bool	horde_pick_spawn(t_horde *h, const t_grid *g, const t_level *lv,
		t_vec2 *out)
{
	int	i;

	if (lv->n_spawns == 0 || h->flow_cell < 0)
		return (false);
	i = pick(h, g, lv, true);
	if (i < 0)
		i = pick(h, g, lv, false);
	if (i < 0)
		return (false);
	*out = (t_vec2){lv->spawns[i].x + 0.5f, lv->spawns[i].y + 0.5f};
	return (true);
}

/* Une place libre, sinon le corps suivant à partir de reuse ; -1 si aucune */
static int	free_slot(t_horde *h)
{
	int	k;
	int	i;

	if (h->n < h->cap)
		return (h->n++);
	k = -1;
	while (++k < h->n)
	{
		i = (h->reuse + k) % h->n;
		if (h->v[i].state == EN_DEAD)
		{
			h->reuse = (i + 1) % h->n;
			return (i);
		}
	}
	return (-1);
}

bool	horde_spawn(t_horde *h, t_vec2 pos, t_breed breed)
{
	t_enemy	*e;
	int		i;

	i = free_slot(h);
	if (i < 0)
		return (false);
	e = &h->v[i];
	*e = (t_enemy){0};
	e->pos = pos;
	e->facing = atan2f(h->player.y - pos.y, h->player.x - pos.x);
	e->state = EN_CHASE;
	e->cooldown = ENEMY_REACTION + (horde_rand(h) % 500) / 1000.0;
	e->hp = breed.hp;
	e->speed = breed.speed;
	return (true);
}
