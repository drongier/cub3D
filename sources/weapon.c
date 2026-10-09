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

bool	weapon_flash(const t_weapon *w)
{
	return (w->since_shot < WEAPON_FLASH_TIME);
}

/* Balancement en huit couché (x sur un pas, y sur deux) et recul */
t_vec2	weapon_offset(const t_weapon *w)
{
	t_vec2	o;
	float	k;

	o.x = sinf(w->bob_phase) * WEAPON_BOB_PX * w->bob_amount;
	o.y = (1.0f - cosf(2.0f * w->bob_phase)) * 0.5f * WEAPON_BOB_PX * 0.5f
		* w->bob_amount;
	if (w->since_shot < WEAPON_RECOIL_TIME)
	{
		k = 1.0f - (float)(w->since_shot / WEAPON_RECOIL_TIME);
		o.y += WEAPON_RECOIL_PX * k;
	}
	return (o);
}
