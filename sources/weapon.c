#include "../includes/weapon.h"
#include <math.h>

void	weapon_init(t_weapon *w)
{
	w->since_shot = 1e9;
	w->trigger_was_down = false;
	w->bob_phase = 0.0f;
	w->bob_amount = 0.0f;
}

/* Le balancement suit le déplacement et s'installe ou s'éteint en douceur */
static void	update_bob(t_weapon *w, t_move move, double dt)
{
	float	target;
	float	hz;

	target = 0.0f;
	if (move.forward || move.strafe)
		target = 1.0f;
	if (w->bob_amount < target)
		w->bob_amount = fminf(target, w->bob_amount + WEAPON_BOB_EASE * dt);
	else
		w->bob_amount = fmaxf(target, w->bob_amount - WEAPON_BOB_EASE * dt);
	hz = WEAPON_BOB_HZ;
	if (move.sprint)
		hz *= SPRINT_FACTOR;
	if (target > 0.0f)
		w->bob_phase = fmodf(w->bob_phase + 2.0f * PI_F * hz * (float)dt,
				2.0f * PI_F);
}

bool	weapon_update(t_weapon *w, bool trigger, t_move move, double dt)
{
	bool	fired;

	w->since_shot += dt;
	fired = trigger && !w->trigger_was_down
		&& w->since_shot >= WEAPON_COOLDOWN;
	if (fired)
		w->since_shot = 0.0;
	w->trigger_was_down = trigger;
	update_bob(w, move, dt);
	return (fired);
}

int	weapon_frame(const t_weapon *w)
{
	if (w->since_shot < WEAPON_ANIM_1)
		return (1);
	if (w->since_shot < WEAPON_ANIM_2)
		return (2);
	if (w->since_shot < WEAPON_ANIM_3)
		return (3);
	if (w->since_shot < WEAPON_ANIM_4)
		return (4);
	return (0);
}

/* Balancement en huit couché : x sur un pas, y sur deux */
t_vec2	weapon_offset(const t_weapon *w)
{
	t_vec2	o;

	o.x = sinf(w->bob_phase) * WEAPON_BOB_PX * w->bob_amount;
	o.y = (1.0f - cosf(2.0f * w->bob_phase)) * 0.5f * WEAPON_BOB_PX * 0.5f
		* w->bob_amount;
	return (o);
}
