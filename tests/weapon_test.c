#include "../includes/weapon.h"
#include <math.h>
#include <stdio.h>

static int	g_fail;

#define CHECK(cond, ...) do { if (!(cond)) { g_fail++; \
	printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); \
	printf("\n"); } } while (0)

static const t_move	g_still = {0, 0, false};
static const t_move	g_walk = {1, 0, false};

static void	fire_cases(void)
{
	t_weapon	w;
	int			shots;

	weapon_init(&w);
	CHECK(!weapon_flash(&w), "no flash before the first shot");
	CHECK(weapon_update(&w, true, g_still, 0.016), "press fires");
	CHECK(weapon_flash(&w), "flash right after the shot");
	CHECK(!weapon_update(&w, true, g_still, 0.016), "holding does not refire");
	weapon_update(&w, false, g_still, 0.016);
	CHECK(!weapon_update(&w, true, g_still, 0.016),
		"a new press inside the cooldown does not fire");
	weapon_update(&w, false, g_still, 0.3);
	CHECK(weapon_update(&w, true, g_still, 0.016),
		"a new press after the cooldown fires");
	weapon_update(&w, false, g_still, 0.1);
	CHECK(!weapon_flash(&w), "flash is over after 0.1 s");
	weapon_init(&w);
	shots = 0;
	for (int i = 0; i < 600; i++)
		shots += weapon_update(&w, i % 2 == 0, g_still, 1.0 / 120);
	CHECK(shots >= 19 && shots <= 21,
		"mashing for 5 s fires at most 4 shots per second: %d", shots);
}

static void	offset_cases(void)
{
	t_weapon	w;
	t_vec2		o;
	float		max_x;

	weapon_init(&w);
	o = weapon_offset(&w);
	CHECK(o.x == 0.0f && o.y == 0.0f, "idle and still: no offset");
	weapon_update(&w, true, g_still, 0.0);
	o = weapon_offset(&w);
	CHECK(fabsf(o.y - WEAPON_RECOIL_PX) < 0.01f, "recoil pushes the gun down");
	weapon_update(&w, false, g_still, WEAPON_RECOIL_TIME);
	CHECK(fabsf(weapon_offset(&w).y) < 0.01f, "recoil is over");
	max_x = 0.0f;
	for (int i = 0; i < 240; i++)
	{
		weapon_update(&w, false, g_walk, 1.0 / 120);
		o = weapon_offset(&w);
		CHECK(o.y >= -0.001f, "bob never lifts the gun off the bottom");
		if (fabsf(o.x) > max_x)
			max_x = fabsf(o.x);
	}
	CHECK(max_x > WEAPON_BOB_PX * 0.8f && max_x <= WEAPON_BOB_PX + 0.01f,
		"walking sways the gun: %f", max_x);
	for (int i = 0; i < 240; i++)
		weapon_update(&w, false, g_still, 1.0 / 120);
	o = weapon_offset(&w);
	CHECK(fabsf(o.x) < 0.5f && fabsf(o.y) < 0.5f,
		"the sway dies out when standing still: %f %f", o.x, o.y);
}

int	main(void)
{
	fire_cases();
	offset_cases();
	if (g_fail)
		printf("weapon_test: %d check(s) failed\n", g_fail);
	else
		printf("weapon_test: ok\n");
	return (g_fail != 0);
}
