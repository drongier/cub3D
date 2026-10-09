#include "../../includes/enemy.h"
#include <math.h>
#include <stdlib.h>

bool	horde_init(t_horde *h, const t_level *lv)
{
	int	i;

	h->n = lv->n_enemies;
	h->rng = 0x2545F491u;
	h->flow_cell = -1;
	h->v = calloc(h->n + 1, sizeof(t_enemy));
	h->flow = malloc(sizeof(int) * ((size_t)lv->w * lv->h + 1));
	h->queue = malloc(sizeof(int) * ((size_t)lv->w * lv->h + 1));
	if (!h->v || !h->flow || !h->queue)
		return (horde_free(h), false);
	i = -1;
	while (++i < h->n)
	{
		h->v[i].pos = (t_vec2){lv->enemies[i].x + 0.5f,
			lv->enemies[i].y + 0.5f};
		h->v[i].facing = PI_F / 2;
		h->v[i].state = EN_IDLE;
		h->v[i].hp = ENEMY_HP;
	}
	return (true);
}

void	horde_free(t_horde *h)
{
	free(h->v);
	free(h->flow);
	free(h->queue);
	h->v = NULL;
	h->flow = NULL;
	h->queue = NULL;
	h->n = 0;
}

/* xorshift32 : même graine, même partie */
uint32_t	horde_rand(t_horde *h)
{
	h->rng ^= h->rng << 13;
	h->rng ^= h->rng >> 17;
	h->rng ^= h->rng << 5;
	return (h->rng);
}

int	horde_alive(const t_horde *h)
{
	int	i;
	int	n;

	n = 0;
	i = -1;
	while (++i < h->n)
		n += h->v[i].state != EN_DYING && h->v[i].state != EN_DEAD;
	return (n);
}

static float	wrap_angle(float a)
{
	while (a > PI_F)
		a -= 2 * PI_F;
	while (a < -PI_F)
		a += 2 * PI_F;
	return (a);
}

/* Le joueur est-il devant lui, assez près, sans mur entre eux ? */
static bool	sees(const t_enemy *e, const t_grid *g, t_vec2 p)
{
	t_vec2	d;

	d = (t_vec2){p.x - e->pos.x, p.y - e->pos.y};
	if (hypotf(d.x, d.y) > ENEMY_SIGHT)
		return (false);
	if (e->state == EN_IDLE && fabsf(wrap_angle(atan2f(d.y, d.x)
				- e->facing)) > ENEMY_SIGHT_HALF_ANGLE)
		return (false);
	return (clear_line(g, e->pos, p));
}

static void	wake(t_horde *h, t_enemy *e)
{
	e->state = EN_CHASE;
	e->t = 0.0;
	e->cooldown = ENEMY_REACTION + (horde_rand(h) % 500) / 1000.0;
}

/* Un tir : touché selon la distance, dégâts entre MIN et MIN + RAND - 1 */
static int	fire(t_horde *h, const t_enemy *e, t_vec2 p)
{
	float	chance;

	chance = 0.85f - 0.05f * hypotf(p.x - e->pos.x, p.y - e->pos.y);
	if (chance < 0.2f)
		chance = 0.2f;
	if ((horde_rand(h) % 1000) >= chance * 1000)
		return (0);
	return (ENEMY_DAMAGE_MIN + horde_rand(h) % ENEMY_DAMAGE_RAND);
}

static int	attack(t_horde *h, t_enemy *e, t_target p, double dt)
{
	double	before;
	int		dmg;

	before = e->t;
	e->t += dt;
	e->facing = atan2f(p.pos.y - e->pos.y, p.pos.x - e->pos.x);
	dmg = 0;
	if (p.alive && before < ENEMY_SHOT_1 && e->t >= ENEMY_SHOT_1)
		dmg += fire(h, e, p.pos);
	if (p.alive && before < ENEMY_SHOT_2 && e->t >= ENEMY_SHOT_2)
		dmg += fire(h, e, p.pos);
	if (e->t >= ENEMY_ATTACK_END)
	{
		e->state = EN_CHASE;
		e->t = 0.0;
		e->cooldown = ENEMY_ATTACK_PAUSE + (horde_rand(h) % 800) / 1000.0;
	}
	return (dmg);
}

static void	chase(t_horde *h, int i, const t_grid *g, t_target p, double dt)
{
	t_enemy	*e;

	e = &h->v[i];
	e->cooldown -= dt;
	if (p.alive && e->cooldown <= 0.0 && sees(e, g, p.pos)
		&& hypotf(p.pos.x - e->pos.x, p.pos.y - e->pos.y) < ENEMY_ATTACK_RANGE)
	{
		e->state = EN_ATTACK;
		e->t = 0.0;
		e->moving = false;
		return ;
	}
	enemy_move(h, i, g, p.pos, dt);
}

static int	step(t_horde *h, int i, const t_grid *g, t_target p, double dt)
{
	t_enemy	*e;

	e = &h->v[i];
	if (e->state == EN_IDLE && p.alive && (sees(e, g, p.pos) || (p.fired
				&& hypotf(p.pos.x - e->pos.x, p.pos.y - e->pos.y)
				< ENEMY_HEARING)))
		wake(h, e);
	else if (e->state == EN_CHASE)
		chase(h, i, g, p, dt);
	else if (e->state == EN_ATTACK)
		return (attack(h, e, p, dt));
	else if (e->state == EN_PAIN || e->state == EN_DYING)
	{
		e->t += dt;
		if (e->state == EN_PAIN && e->t >= ENEMY_PAIN_TIME)
			wake(h, e);
		else if (e->state == EN_DYING && e->t >= 6 * ENEMY_DEATH_FRAME)
			e->state = EN_DEAD;
	}
	return (0);
}

int	horde_update(t_horde *h, const t_grid *g, t_target target, double dt)
{
	int	dmg;
	int	i;

	if (h->n == 0)
		return (0);
	flow_update(h, g, target.pos);
	dmg = 0;
	i = -1;
	while (++i < h->n)
	{
		h->v[i].moving = false;
		dmg += step(h, i, g, target, dt);
	}
	return (dmg);
}
