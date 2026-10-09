#include "../includes/motion.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

static int	g_fail;

#define CHECK(cond, ...) do { if (!(cond)) { g_fail++; \
	printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); \
	printf("\n"); } } while (0)

static bool	near(double a, double b)
{
	return (fabs(a - b) < 1e-4);
}

int	main(void)
{
	t_vec2	v;
	t_vec2	w;

	CHECK(near(motion_clamp_dt(0.016), 0.016), "small dt kept");
	CHECK(near(motion_clamp_dt(2.0), MOTION_MAX_DT), "large dt clamped");
	CHECK(near(motion_clamp_dt(-1.0), 0.0), "negative dt is zero");

	v = motion_step(0.0f, (t_move){1, 0, false}, 1.0);
	CHECK(near(v.x, MOVE_SPEED) && near(v.y, 0.0), "forward at angle 0: %f %f",
		v.x, v.y);
	v = motion_step(0.0f, (t_move){0, 1, false}, 1.0);
	CHECK(near(v.x, 0.0) && near(v.y, MOVE_SPEED), "strafe right at angle 0");
	v = motion_step(0.0f, (t_move){1, 1, false}, 1.0);
	CHECK(near(hypot(v.x, v.y), MOVE_SPEED), "diagonal is not faster: %f",
		hypot(v.x, v.y));
	v = motion_step(1.0f, (t_move){0, 0, false}, 1.0);
	CHECK(v.x == 0.0f && v.y == 0.0f, "no key, no move");
	v = motion_step(1.0f, (t_move){1, 0, false}, 0.01);
	w = motion_step(1.0f, (t_move){1, 0, false}, 0.02);
	CHECK(near(w.x, 2 * v.x) && near(w.y, 2 * v.y), "distance follows dt");

	v = motion_step(0.0f, (t_move){1, 0, true}, 1.0);
	CHECK(near(v.x, MOVE_SPEED * SPRINT_FACTOR) && near(v.y, 0.0),
		"sprint is SPRINT_FACTOR times faster: %f", v.x);
	v = motion_step(0.7f, (t_move){1, -1, true}, 1.0);
	CHECK(near(hypot(v.x, v.y), MOVE_SPEED * SPRINT_FACTOR),
		"sprinting diagonally is not faster: %f", hypot(v.x, v.y));
	v = motion_step(0.7f, (t_move){0, 0, true}, 1.0);
	CHECK(v.x == 0.0f && v.y == 0.0f, "sprint alone does not move");
	CHECK(near(motion_turn(0.0f, 1, 0.5), ROT_SPEED * 0.5), "turn right");
	CHECK(near(motion_turn(0.1f, -1, 0.5), 2 * PI_F + 0.1 - ROT_SPEED * 0.5),
		"turn left wraps above 0");
	CHECK(near(motion_turn(2 * PI_F - 0.1f, 1, 0.1), ROT_SPEED * 0.1 - 0.1),
		"turn right wraps below 2pi");
	CHECK(near(ROT_SPEED / 60.0, 0.03) && near(MOVE_SPEED / 60.0, 3.0),
		"same speed as before at 60 Hz");
	if (g_fail)
		printf("motion_test: %d check(s) failed\n", g_fail);
	else
		printf("motion_test: ok\n");
	return (g_fail != 0);
}
