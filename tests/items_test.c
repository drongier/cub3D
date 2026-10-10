#include "../includes/items.h"
#include <stdio.h>
#include <string.h>

static int	g_fail;

#define CHECK(cond, ...) do { if (!(cond)) { g_fail++; \
	printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); \
	printf("\n"); } } while (0)

static void	setup(t_items *it, t_level *lv, t_level_enemy *spots, int n)
{
	memset(lv, 0, sizeof(*lv));
	lv->items = spots;
	lv->n_items = n;
	items_init(it, lv);
}

static void	pickup_cases(void)
{
	t_level_enemy	s[2] = {{2, 2, 1}, {5, 2, 1}};
	t_level			lv;
	t_items			it;

	setup(&it, &lv, s, 2);
	CHECK(it.n == 2 && it.v[0].present && it.v[0].pos.x == 2.5f,
		"one kit per H, at the cell center");
	CHECK(items_pickup(&it, (t_vec2){2.5f, 2.5f}, 100, 100) == 0
		&& it.v[0].present, "left alone when health is full");
	CHECK(items_pickup(&it, (t_vec2){3.5f, 2.5f}, 40, 100) == 0,
		"too far to reach");
	CHECK(items_pickup(&it, (t_vec2){2.8f, 2.6f}, 40, 100) == MEDKIT_HEAL
		&& !it.v[0].present && it.v[1].present, "picked up when hurt");
	CHECK(items_pickup(&it, (t_vec2){2.5f, 2.5f}, 40, 100) == 0,
		"gone once taken");
	CHECK(items_pickup(&it, (t_vec2){5.5f, 2.5f}, 90, 100) == 10,
		"heals up to the maximum only");
	items_free(&it);
	CHECK(it.v == NULL && it.n == 0, "freed");
}

static void	respawn_cases(void)
{
	t_level_enemy	s[2] = {{2, 2, 1}, {5, 2, 1}};
	t_level			lv;
	t_items			it;
	int				i;

	setup(&it, &lv, s, 2);
	items_pickup(&it, (t_vec2){2.5f, 2.5f}, 10, 100);
	i = -1;
	while (++i < (int)(MEDKIT_RESPAWN * 60) - 2)
		items_update(&it, 1.0 / 60);
	CHECK(!it.v[0].present, "not back before the delay");
	items_update(&it, 3.0 / 60);
	CHECK(it.v[0].present, "back after the delay");
	items_pickup(&it, (t_vec2){2.5f, 2.5f}, 10, 100);
	items_pickup(&it, (t_vec2){5.5f, 2.5f}, 10, 100);
	items_restock(&it);
	CHECK(it.v[0].present && it.v[1].present, "restock brings all back");
	items_free(&it);
	setup(&it, &lv, s, 0);
	CHECK(it.n == 0 && items_pickup(&it, (t_vec2){2.5f, 2.5f}, 10, 100) == 0,
		"a scene without kits");
	items_free(&it);
}

int	main(void)
{
	pickup_cases();
	respawn_cases();
	if (g_fail)
		printf("items_test: %d check(s) failed\n", g_fail);
	else
		printf("items_test: ok\n");
	return (g_fail != 0);
}
