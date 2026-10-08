#include "../includes/options.h"
#include <stdio.h>
#include <string.h>

static int	g_fail;

#define CHECK(cond, ...) do { if (!(cond)) { g_fail++; \
	printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); \
	printf("\n"); } } while (0)

#define ARGC(a) ((int)(sizeof(a) / sizeof(a[0])))

int	main(void)
{
	t_options	o;
	char		*a1[] = {"cub3D", "map.cub"};
	char		*a2[] = {"cub3D", "--no-vsync", "map.cub"};
	char		*a3[] = {"cub3D", "map.cub", "--bench"};
	char		*a4[] = {"cub3D", "--bench", "200", "map.cub"};
	char		*a5[] = {"cub3D", "--bench", "map.cub"};
	char		*b1[] = {"cub3D"};
	char		*b2[] = {"cub3D", "a.cub", "b.cub"};
	char		*b3[] = {"cub3D", "--fast", "a.cub"};
	char		*b4[] = {"cub3D", "--bench", "0", "a.cub"};
	char		*b5[] = {"cub3D", "--bench", "99999999", "a.cub"};
	char		*b6[] = {"cub3D", "--bench", "200"};
	char		*a6[] = {"cub3D", "--fps", "30", "map.cub"};
	char		*b7[] = {"cub3D", "--fps", "9", "a.cub"};
	char		*b8[] = {"cub3D", "--fps", "1001", "a.cub"};
	char		*b9[] = {"cub3D", "a.cub", "--fps"};
	char		*b10[] = {"cub3D", "--fps", "abc", "a.cub"};

	CHECK(parse_options(ARGC(a1), a1, &o) && o.vsync && o.bench_frames == 0
		&& strcmp(o.scene_path, "map.cub") == 0, "plain map");
	CHECK(parse_options(ARGC(a2), a2, &o) && !o.vsync && o.bench_frames == 0,
		"--no-vsync");
	CHECK(parse_options(ARGC(a3), a3, &o) && o.bench_frames == 1000 && !o.vsync,
		"--bench after the map, default 1000, vsync off");
	CHECK(parse_options(ARGC(a4), a4, &o) && o.bench_frames == 200
		&& strcmp(o.scene_path, "map.cub") == 0, "--bench 200");
	CHECK(parse_options(ARGC(a5), a5, &o) && o.bench_frames == 1000
		&& strcmp(o.scene_path, "map.cub") == 0, "--bench then map");
	CHECK(parse_options(ARGC(a1), a1, &o) && o.fps_cap == 0, "no cap by default");
	CHECK(parse_options(ARGC(a6), a6, &o) && o.fps_cap == 30
		&& strcmp(o.scene_path, "map.cub") == 0, "--fps 30");
	CHECK(!parse_options(ARGC(b7), b7, &o), "--fps too low");
	CHECK(!parse_options(ARGC(b8), b8, &o), "--fps too high");
	CHECK(!parse_options(ARGC(b9), b9, &o), "--fps without value");
	CHECK(!parse_options(ARGC(b10), b10, &o), "--fps not a number");
	CHECK(!parse_options(ARGC(b1), b1, &o), "no map");
	CHECK(!parse_options(ARGC(b2), b2, &o), "two maps");
	CHECK(!parse_options(ARGC(b3), b3, &o), "unknown option");
	CHECK(!parse_options(ARGC(b4), b4, &o), "--bench 0");
	CHECK(!parse_options(ARGC(b5), b5, &o), "--bench too large");
	CHECK(!parse_options(ARGC(b6), b6, &o), "--bench N without map");
	if (g_fail)
		printf("options_test: %d check(s) failed\n", g_fail);
	else
		printf("options_test: ok\n");
	return (g_fail != 0);
}
