#include "../includes/motion.h"
#include <math.h>

double	motion_clamp_dt(double dt)
{
	if (dt < 0.0)
		return (0.0);
	if (dt > MOTION_MAX_DT)
		return (MOTION_MAX_DT);
	return (dt);
}

/* La direction est normalisée : en diagonale on ne va pas plus vite */
t_vec2	motion_step(float angle, t_move move, double dt)
{
	t_vec2	d;
	float	len;
	float	speed;
	float	c;
	float	s;

	c = cosf(angle);
	s = sinf(angle);
	d.x = move.forward * c - move.strafe * s;
	d.y = move.forward * s + move.strafe * c;
	len = sqrtf(d.x * d.x + d.y * d.y);
	if (len == 0.0f)
		return ((t_vec2){0.0f, 0.0f});
	speed = MOVE_SPEED;
	if (move.sprint)
		speed *= SPRINT_FACTOR;
	d.x = d.x / len * speed * (float)dt;
	d.y = d.y / len * speed * (float)dt;
	return (d);
}

/* turn : +1 droite, -1 gauche ; l'angle reste dans [0, 2pi) */
float	motion_turn(float angle, int turn, double dt)
{
	angle += turn * ROT_SPEED * (float)dt;
	if (angle >= 2 * PI_F)
		angle -= 2 * PI_F;
	if (angle < 0.0f)
		angle += 2 * PI_F;
	return (angle);
}
