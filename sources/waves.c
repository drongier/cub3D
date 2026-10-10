#include "../includes/waves.h"
#include <math.h>

t_wave_rule	wave_rule(int index)
{
	t_wave_rule	r;

	r.count = 2 + 2 * index;
	if (r.count > WAVE_MAX_COUNT)
		r.count = WAVE_MAX_COUNT;
	r.interval = fmax(0.35, 1.4 - 0.1 * index);
	r.breed.hp = ENEMY_HP + 5 * (index - 1);
	if (r.breed.hp > 2 * ENEMY_HP)
		r.breed.hp = 2 * ENEMY_HP;
	r.breed.speed = fminf(ENEMY_SPEED + 0.08f * (index - 1), 2.8f);
	return (r);
}

void	waves_start(t_waves *w)
{
	*w = (t_waves){0};
	w->index = 1;
	w->state = WAVE_PAUSE;
	w->pause = WAVE_FIRST_DELAY;
	w->rule = wave_rule(1);
}

bool	waves_update(t_waves *w, int alive, double dt)
{
	w->t += dt;
	if (w->state == WAVE_PAUSE && w->t >= w->pause)
	{
		w->state = WAVE_SPAWN;
		w->rule = wave_rule(w->index);
		w->spawned = 0;
		w->t = w->rule.interval;
	}
	else if (w->state == WAVE_SPAWN && w->spawned >= w->rule.count)
		w->state = WAVE_FIGHT;
	else if (w->state == WAVE_FIGHT && alive == 0)
	{
		w->index++;
		w->state = WAVE_PAUSE;
		w->pause = WAVE_BREAK;
		w->t = 0.0;
		return (true);
	}
	return (false);
}

bool	waves_due(const t_waves *w)
{
	return (w->state == WAVE_SPAWN && w->spawned < w->rule.count
		&& w->t >= w->rule.interval);
}

void	waves_spawned(t_waves *w)
{
	w->spawned++;
	w->t = 0.0;
}

double	waves_countdown(const t_waves *w)
{
	if (w->state != WAVE_PAUSE)
		return (0.0);
	return (fmax(0.0, w->pause - w->t));
}
