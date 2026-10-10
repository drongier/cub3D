#include "../includes/waves.h"
#include <stdio.h>

static int	g_fail;

#define CHECK(cond, ...) do { if (!(cond)) { g_fail++; \
	printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); \
	printf("\n"); } } while (0)

#define DT (1.0 / 60)

/* Avance de secs secondes ; compte les apparitions et les fins de vague */
static void	run(t_waves *w, int *alive, double secs, int *spawns, int *ends)
{
	int	i;

	i = -1;
	while (++i < (int)(secs / DT + 0.5))
	{
		*ends += waves_update(w, *alive, DT);
		while (waves_due(w))
		{
			waves_spawned(w);
			(*alive)++;
			(*spawns)++;
		}
	}
}

static void	cycle_cases(void)
{
	t_waves	w;
	int		alive;
	int		spawns;
	int		ends;

	waves_start(&w);
	alive = 0;
	spawns = 0;
	ends = 0;
	CHECK(w.index == 1 && w.state == WAVE_PAUSE, "starts paused on wave 1");
	CHECK(waves_countdown(&w) == WAVE_FIRST_DELAY, "countdown");
	run(&w, &alive, WAVE_FIRST_DELAY - 0.1, &spawns, &ends);
	CHECK(spawns == 0 && waves_countdown(&w) > 0.0, "nothing during pause");
	run(&w, &alive, 0.2, &spawns, &ends);
	CHECK(w.state == WAVE_SPAWN && spawns == 1,
		"first mutant right after the pause: %d", spawns);
	run(&w, &alive, 20.0, &spawns, &ends);
	CHECK(spawns == wave_rule(1).count && w.state == WAVE_FIGHT && ends == 0,
		"the whole wave arrives, then the fight: %d", spawns);
	alive = 1;
	run(&w, &alive, 5.0, &spawns, &ends);
	CHECK(ends == 0 && w.state == WAVE_FIGHT, "not over while one lives");
	alive = 0;
	run(&w, &alive, DT, &spawns, &ends);
	CHECK(ends == 1 && w.index == 2 && w.state == WAVE_PAUSE
		&& waves_countdown(&w) > WAVE_BREAK - 0.1, "cleared: pause, wave 2");
	run(&w, &alive, WAVE_BREAK + 30.0, &spawns, &ends);
	CHECK(spawns == wave_rule(1).count + wave_rule(2).count,
		"wave 2 arrives in full: %d", spawns);
}

/* Les apparitions bloquées (aucun point libre) sont seulement retardées */
static void	blocked_cases(void)
{
	t_waves	w;
	int		i;

	waves_start(&w);
	i = -1;
	while (++i < 600)
		waves_update(&w, 0, DT);
	CHECK(w.state == WAVE_SPAWN && waves_due(&w) && w.spawned == 0,
		"still due while nobody could spawn");
	waves_spawned(&w);
	CHECK(!waves_due(&w), "then waits for the interval");
}

static void	rule_cases(void)
{
	int	n;

	CHECK(wave_rule(1).count == 4 && wave_rule(1).breed.hp == ENEMY_HP
		&& wave_rule(1).breed.speed == ENEMY_SPEED, "wave 1 is the base");
	n = 1;
	while (++n < 100)
	{
		CHECK(wave_rule(n).count >= wave_rule(n - 1).count
			&& wave_rule(n).interval <= wave_rule(n - 1).interval
			&& wave_rule(n).breed.hp >= wave_rule(n - 1).breed.hp
			&& wave_rule(n).breed.speed >= wave_rule(n - 1).breed.speed,
			"never easier than the wave before: %d", n);
	}
	CHECK(wave_rule(99).count == WAVE_MAX_COUNT
		&& wave_rule(99).interval >= 0.35
		&& wave_rule(99).breed.hp == 2 * ENEMY_HP
		&& wave_rule(99).breed.speed <= 2.8f, "capped");
}

int	main(void)
{
	cycle_cases();
	blocked_cases();
	rule_cases();
	if (g_fail)
		printf("waves_test: %d check(s) failed\n", g_fail);
	else
		printf("waves_test: ok\n");
	return (g_fail != 0);
}
