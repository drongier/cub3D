#include "../includes/raycast.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

static int	g_fail;

#define CHECK(cond, ...) do { if (!(cond)) { g_fail++; \
	printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); \
	printf("\n"); } } while (0)

static bool	near(double a, double b)
{
	return (fabs(a - b) < 1e-3);
}

int	main(void)
{
	const char	*rows = "11111" "10001" "10001" "10001" "11111";
	const char	*open_rows = "  111" " 1001" "11001" "10001" "11111";
	t_grid		g;
	t_hit		h;
	t_camera	cam;
	t_vec2		r;

	CHECK(grid_init(&g, rows, 5, 5), "grid init");
	CHECK(grid_at(&g, 0, 0) == CELL_WALL && grid_at(&g, 2, 2) == CELL_FLOOR
		&& grid_at(&g, -1, 2) == CELL_WALL && grid_at(&g, 5, 2) == CELL_WALL,
		"grid cells and outside");

	h = cast_ray(&g, (t_vec2){2.5f, 2.25f}, (t_vec2){1, 0});
	CHECK(near(h.dist, 1.5) && h.face == FACE_EAST && near(h.wall_x, 0.25)
		&& near(h.point.x, 4) && near(h.point.y, 2.25), "east: %f %d %f",
		h.dist, h.face, h.wall_x);
	h = cast_ray(&g, (t_vec2){2.5f, 2.25f}, (t_vec2){-1, 0});
	CHECK(near(h.dist, 1.5) && h.face == FACE_WEST && near(h.wall_x, 0.75),
		"west is mirrored: %f %d %f", h.dist, h.face, h.wall_x);
	h = cast_ray(&g, (t_vec2){2.25f, 2.5f}, (t_vec2){0, 1});
	CHECK(near(h.dist, 1.5) && h.face == FACE_SOUTH && near(h.wall_x, 0.75),
		"south is mirrored: %f %d %f", h.dist, h.face, h.wall_x);
	h = cast_ray(&g, (t_vec2){2.25f, 2.5f}, (t_vec2){0, -1});
	CHECK(near(h.dist, 1.5) && h.face == FACE_NORTH && near(h.wall_x, 0.25),
		"north: %f %d %f", h.dist, h.face, h.wall_x);
	h = cast_ray(&g, (t_vec2){2.5f, 2.5f}, (t_vec2){1, 0.5f});
	CHECK(near(h.dist, 1.5) && near(h.point.x, 4) && near(h.point.y, 3.25),
		"oblique ray keeps the perpendicular distance: %f (%f, %f)", h.dist,
		h.point.x, h.point.y);

	cam = camera_make((t_vec2){2.5f, 2.5f}, 0.0f, 1280);
	CHECK(near(cam.focal, 640 / tan(FOV_DEG / 2 * M_PI / 180)),
		"focal: %f", cam.focal);
	r = camera_ray(&cam, 640, 1280);
	CHECK(r.x > 0.999f && fabsf(r.y) < 0.01f, "center column looks ahead");
	r = camera_ray(&cam, 0, 1280);
	CHECK(r.y < 0 && fabs(atan2(r.y, r.x) + FOV_DEG / 2 * M_PI / 180) < 0.01,
		"left edge at -FOV/2: %f", atan2(r.y, r.x) * 180 / M_PI);
	CHECK(wall_height(&cam, 1.0f) == (int)cam.focal,
		"a block one cell away is as tall as it is wide on screen");
	CHECK(wall_height(&cam, 0.0f) > 0, "zero distance does not divide by 0");
	grid_free(&g);

	CHECK(grid_init(&g, open_rows, 5, 5), "grid init");
	CHECK(grid_at(&g, 0, 0) == CELL_VOID && grid_at(&g, 1, 0) == CELL_VOID
		&& grid_at(&g, 2, 2) == CELL_FLOOR, "outside spaces are void");
	grid_free(&g);
	if (g_fail)
		printf("raycast_test: %d check(s) failed\n", g_fail);
	else
		printf("raycast_test: ok\n");
	return (g_fail != 0);
}
