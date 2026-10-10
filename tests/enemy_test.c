#include "../includes/enemy.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

static int	g_fail;

#define CHECK(cond, ...) do { if (!(cond)) { g_fail++; \
	printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); \
	printf("\n"); } } while (0)

/*
 * Couloir en L :
 *   1111111
 *   1.....1   y = 1
 *   11111.1
 *   1.....1   y = 3
 *   1111111
 */
static const char	*g_cells = "1111111" "1000001" "1111101" "1000001"
	"1111111";

static void	setup(t_grid *g, t_horde *h, t_level *lv, t_level_enemy *spawn,
		int n)
{
	memset(lv, 0, sizeof(*lv));
	lv->w = 7;
	lv->h = 5;
	lv->enemies = spawn;
	lv->n_enemies = n;
	grid_init(g, g_cells, 7, 5);
	horde_init(h, lv);
}

static t_target	target(float x, float y, bool fired)
{
	return ((t_target){{x, y}, true, fired});
}

static void	flow_cases(void)
{
	t_grid			g;
	t_horde			h;
	t_level			lv;
	t_level_enemy	s[1] = {{1, 3, 1}};

	setup(&g, &h, &lv, s, 1);
	flow_update(&h, &g, (t_vec2){1.5f, 1.5f});
	CHECK(h.flow[1 * 7 + 1] == 0 && h.flow[1 * 7 + 5] == 4
		&& h.flow[2 * 7 + 5] == 5 && h.flow[3 * 7 + 1] == 10
		&& h.flow[0] == -1, "flow distances along the corridor");
	horde_free(&h);
	grid_free(&g);
}

static void	sight_cases(void)
{
	t_grid			g;
	t_horde			h;
	t_level			lv;
	t_level_enemy	s[1] = {{5, 1, 1}};

	setup(&g, &h, &lv, s, 1);
	CHECK(clear_line(&g, (t_vec2){1.5f, 1.5f}, (t_vec2){5.5f, 1.5f}),
		"clear line along the top corridor");
	CHECK(!clear_line(&g, (t_vec2){1.5f, 1.5f}, (t_vec2){1.5f, 3.5f}),
		"no line through the wall");
	h.v[0].facing = PI_F;
	horde_update(&h, &g, target(1.5f, 1.5f, false), 0.01);
	CHECK(h.v[0].state == EN_CHASE, "sees the player in front: %d",
		h.v[0].state);
	horde_free(&h);
	setup(&g, &h, &lv, s, 1);
	h.v[0].facing = 0.0f;
	horde_update(&h, &g, target(1.5f, 1.5f, false), 0.01);
	CHECK(h.v[0].state == EN_IDLE, "player behind: still idle");
	horde_update(&h, &g, target(1.5f, 1.5f, true), 0.01);
	CHECK(h.v[0].state == EN_CHASE, "hears the shot");
	horde_free(&h);
	setup(&g, &h, &lv, s, 1);
	h.v[0].facing = PI_F / 2;
	horde_update(&h, &g, target(1.5f, 3.5f, false), 0.01);
	CHECK(h.v[0].state == EN_IDLE, "player behind a wall: idle");
	horde_free(&h);
	grid_free(&g);
}

static void	chase_cases(void)
{
	t_grid			g;
	t_horde			h;
	t_level			lv;
	t_level_enemy	s[1] = {{1, 3, 1}};
	int				i;
	bool			in_wall;
	float			d;

	setup(&g, &h, &lv, s, 1);
	h.v[0].state = EN_CHASE;
	h.v[0].cooldown = 1e9;
	in_wall = false;
	i = -1;
	while (++i < 1200)
	{
		horde_update(&h, &g, target(1.5f, 1.5f, false), 1.0 / 120);
		in_wall |= grid_at(&g, (int)floorf(h.v[0].pos.x),
				(int)floorf(h.v[0].pos.y)) == CELL_WALL;
	}
	d = hypotf(h.v[0].pos.x - 1.5f, h.v[0].pos.y - 1.5f);
	CHECK(!in_wall, "never walks into a wall");
	CHECK(d >= ENEMY_KEEP_AWAY - 0.01f && d < ENEMY_KEEP_AWAY + 0.3f,
		"walked around the corner up to the player: %f", d);
	horde_free(&h);
	grid_free(&g);
}

/* Une salle 7 x 5 ; un mutant immobile au milieu du chemin le plus court */
static void	detour_cases(void)
{
	const char		*room = "1111111" "1000001" "1000001" "1000001"
		"1111111";
	t_grid			g;
	t_horde			h;
	t_level			lv;
	t_level_enemy	s[2] = {{5, 2, 1}, {3, 2, 1}};
	int				i;

	memset(&lv, 0, sizeof(lv));
	lv.w = 7;
	lv.h = 5;
	lv.enemies = s;
	lv.n_enemies = 2;
	grid_init(&g, room, 7, 5);
	horde_init(&h, &lv);
	h.v[0].state = EN_CHASE;
	h.v[0].cooldown = 1e9;
	h.v[1].facing = 0.0f;
	i = -1;
	while (++i < 600)
		horde_update(&h, &g, target(1.5f, 2.5f, false), 1.0 / 120);
	CHECK(h.v[1].state == EN_IDLE, "the one in the way stays idle");
	CHECK(h.v[0].pos.x < 3.0f, "walks around the idle one: x = %f",
		h.v[0].pos.x);
	horde_free(&h);
	grid_free(&g);
}

static void	attack_cases(void)
{
	t_grid			g;
	t_horde			h;
	t_level			lv;
	t_level_enemy	s[1] = {{5, 1, 1}};
	int				dmg;
	int				i;

	setup(&g, &h, &lv, s, 1);
	h.v[0].facing = PI_F;
	dmg = 0;
	i = -1;
	while (++i < 1200)
		dmg += horde_update(&h, &g, target(2.5f, 1.5f, false), 1.0 / 120);
	CHECK(dmg > 0, "shoots the player in sight within 10 s");
	CHECK(dmg < 100 * 3, "but not absurdly fast: %d", dmg);
	horde_free(&h);
	setup(&g, &h, &lv, s, 1);
	h.v[0].state = EN_CHASE;
	dmg = 0;
	i = -1;
	while (++i < 240)
	{
		h.v[0].pos = (t_vec2){5.5f, 1.5f};
		dmg += horde_update(&h, &g, target(1.5f, 3.5f, false), 1.0 / 120);
	}
	CHECK(dmg == 0, "never shoots through a wall: %d", dmg);
	horde_free(&h);
	grid_free(&g);
}

static void	shoot_cases(void)
{
	t_grid			g;
	t_horde			h;
	t_level			lv;
	t_level_enemy	s[2] = {{4, 1, 1}, {5, 1, 1}};
	int				i;

	setup(&g, &h, &lv, s, 2);
	CHECK(horde_shoot(&h, &g, (t_vec2){1.5f, 1.5f}, 0.0f) == 0,
		"hits the nearest enemy on the line");
	CHECK(h.v[0].hp < ENEMY_HP && h.v[1].hp == ENEMY_HP, "only that one");
	CHECK(h.v[0].state == EN_PAIN, "the hit one flinches");
	CHECK(horde_shoot(&h, &g, (t_vec2){1.5f, 1.5f}, PI_F / 2) == -1,
		"missing to the side hits nobody");
	CHECK(horde_shoot(&h, &g, (t_vec2){1.5f, 3.5f}, 0.0f) == -1,
		"no enemy in the other corridor");
	i = 0;
	while (h.v[0].state != EN_DYING && h.v[0].state != EN_DEAD && i++ < 10)
		horde_shoot(&h, &g, (t_vec2){1.5f, 1.5f}, 0.0f);
	CHECK(i >= 1 && i <= 3 && h.v[0].state == EN_DYING,
		"dies after 2 or 3 shots: %d", i + 1);
	CHECK(horde_alive(&h) == 1, "one left alive");
	CHECK(horde_shoot(&h, &g, (t_vec2){1.5f, 1.5f}, 0.0f) == 1,
		"shots go through the dying one to the next");
	i = -1;
	while (++i < 240)
		horde_update(&h, &g, target(1.5f, 3.5f, false), 1.0 / 120);
	CHECK(h.v[0].state == EN_DEAD && enemy_cell(&h.v[0], (t_vec2){0, 0}).col
		== 6, "the death animation ends on the corpse");
	CHECK(!horde_blocks(&h, h.v[0].pos, 0.2f), "corpses do not block");
	CHECK(horde_blocks(&h, h.v[1].pos, 0.2f), "living enemies do");
	horde_free(&h);
	grid_free(&g);
}

/*
 * Salle 11 x 5 et deux points : (1, 1), à 7 cases de marche du joueur et
 * caché, et (9, 3), à 3 cases et en vue.
 *   11111111111
 *   1X000000001
 *   10111111101
 *   100000P00X1    P : le joueur en (6, 3)
 *   11111111111
 */
static void	spawn_cases(void)
{
	const char		*room = "11111111111" "10000000001" "10111111101"
		"10000000001" "11111111111";
	t_grid			g;
	t_horde			h;
	t_level			lv;
	t_level_enemy	pts[2] = {{1, 1, 1}, {9, 3, 1}};
	t_vec2			p;
	int				i;

	memset(&lv, 0, sizeof(lv));
	lv.w = 11;
	lv.h = 5;
	lv.spawns = pts;
	lv.n_spawns = 2;
	grid_init(&g, room, 11, 5);
	horde_init(&h, &lv);
	CHECK(h.n == 0 && h.cap == HORDE_WAVE_SLOTS, "room kept for the waves");
	CHECK(!horde_pick_spawn(&h, &g, &lv, &p), "no flow yet: no pick");
	horde_update(&h, &g, target(6.5f, 3.5f, false), 0.01);
	CHECK(horde_pick_spawn(&h, &g, &lv, &p) && p.x == 1.5f && p.y == 1.5f,
		"far and hidden wins over the visible one: %f %f", p.x, p.y);
	CHECK(horde_spawn(&h, p, (t_breed){70, 2.0f}) && h.n == 1
		&& h.v[0].state == EN_CHASE && h.v[0].hp == 70
		&& h.v[0].speed == 2.0f, "spawns chasing, with the wave's breed");
	CHECK(horde_pick_spawn(&h, &g, &lv, &p) && p.x == 9.5f,
		"(1, 1) taken: falls back to the visible one");
	h.v[0].pos = (t_vec2){9.5f, 3.5f};
	CHECK(horde_pick_spawn(&h, &g, &lv, &p) && p.x == 1.5f, "and back");
	horde_update(&h, &g, target(2.5f, 1.5f, false), 0.01);
	h.v[0].pos = (t_vec2){9.5f, 3.5f};
	CHECK(!horde_pick_spawn(&h, &g, &lv, &p), "too close or taken: wait");
	horde_free(&h);
	horde_init(&h, &lv);
	horde_update(&h, &g, target(6.5f, 3.5f, false), 0.01);
	i = -1;
	while (++i < HORDE_WAVE_SLOTS)
		horde_spawn(&h, (t_vec2){1.5f, 1.5f}, (t_breed){50, 1.6f});
	CHECK(!horde_spawn(&h, p, (t_breed){50, 1.6f}), "full of living ones");
	h.v[3].state = EN_DEAD;
	h.v[5].state = EN_DEAD;
	CHECK(horde_spawn(&h, (t_vec2){9.5f, 3.5f}, (t_breed){50, 1.6f})
		&& h.v[3].state == EN_CHASE && h.v[3].pos.x == 9.5f
		&& h.n == HORDE_WAVE_SLOTS, "recycles the first corpse");
	CHECK(horde_spawn(&h, p, (t_breed){50, 1.6f}) && h.v[5].state == EN_CHASE,
		"then the next one");
	horde_free(&h);
	grid_free(&g);
}

static void	cell_cases(void)
{
	t_enemy	e;

	memset(&e, 0, sizeof(e));
	e.pos = (t_vec2){5.0f, 5.0f};
	e.facing = 0.0f;
	CHECK(enemy_cell(&e, (t_vec2){8, 5}).col == 0, "seen from the front");
	CHECK(enemy_cell(&e, (t_vec2){2, 5}).col == 4, "seen from behind");
	CHECK(enemy_cell(&e, (t_vec2){5, 8}).col == 6,
		"player on its right side sees it face right");
	CHECK(enemy_cell(&e, (t_vec2){5, 2}).col == 2, "and the other side");
	CHECK(enemy_cell(&e, (t_vec2){8, 8}).col == 7, "three quarters");
	CHECK(enemy_cell(&e, (t_vec2){8, 5}).row == 0, "standing row");
	e.state = EN_ATTACK;
	e.t = ENEMY_SHOT_1 + 0.01;
	CHECK(enemy_cell(&e, (t_vec2){8, 5}).row == 6
		&& enemy_cell(&e, (t_vec2){8, 5}).col == 1, "firing frame");
}

int	main(void)
{
	flow_cases();
	sight_cases();
	chase_cases();
	detour_cases();
	attack_cases();
	shoot_cases();
	spawn_cases();
	cell_cases();
	if (g_fail)
		printf("enemy_test: %d check(s) failed\n", g_fail);
	else
		printf("enemy_test: ok\n");
	return (g_fail != 0);
}
