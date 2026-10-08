#ifndef MOTION_H
# define MOTION_H

/* Vitesses en unités par seconde : 3 px et 0.03 rad par frame à 60 Hz */
# define MOVE_SPEED 180.0f
# define ROT_SPEED 1.8f
/* Au-delà, un blocage ferait faire un bond au joueur */
# define MOTION_MAX_DT 0.05
/* Pas fixe du bench, pour des images identiques d'un run à l'autre */
# define MOTION_BENCH_DT (1.0 / 60.0)
# define PI_F 3.14159265359f

typedef struct s_vec2
{
	float	x;
	float	y;
}	t_vec2;

double	motion_clamp_dt(double dt);
t_vec2	motion_step(float angle, int forward, int strafe, double dt);
float	motion_turn(float angle, int turn, double dt);

#endif
