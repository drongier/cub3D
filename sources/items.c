#include "../includes/items.h"
#include <math.h>
#include <stdlib.h>

bool	items_init(t_items *it, const t_level *lv)
{
	int	i;

	it->n = lv->n_items;
	it->v = calloc(it->n + 1, sizeof(t_item));
	if (!it->v)
	{
		it->n = 0;
		return (false);
	}
	i = -1;
	while (++i < it->n)
	{
		it->v[i].pos = (t_vec2){lv->items[i].x + 0.5f, lv->items[i].y + 0.5f};
		it->v[i].present = true;
	}
	return (true);
}

void	items_free(t_items *it)
{
	free(it->v);
	it->v = NULL;
	it->n = 0;
}

void	items_update(t_items *it, double dt)
{
	int	i;

	i = -1;
	while (++i < it->n)
	{
		if (it->v[i].present)
			continue ;
		it->v[i].t -= dt;
		if (it->v[i].t <= 0.0)
			it->v[i].present = true;
	}
}

void	items_restock(t_items *it)
{
	int	i;

	i = -1;
	while (++i < it->n)
		it->v[i].present = true;
}

int	items_pickup(t_items *it, t_vec2 p, int hp, int max_hp)
{
	int	heal;
	int	i;

	i = -1;
	while (++i < it->n && hp < max_hp)
	{
		if (!it->v[i].present || hypotf(it->v[i].pos.x - p.x,
				it->v[i].pos.y - p.y) > MEDKIT_REACH)
			continue ;
		it->v[i].present = false;
		it->v[i].t = MEDKIT_RESPAWN;
		heal = MEDKIT_HEAL;
		if (hp + heal > max_hp)
			heal = max_hp - hp;
		return (heal);
	}
	return (0);
}
