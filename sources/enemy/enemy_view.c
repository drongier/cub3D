#include "../../includes/enemy.h"
#include <math.h>

/* Distance le long du tir si e est assez près de l'axe, sinon -1 */
static float	along_shot(const t_enemy *e, t_vec2 pos, t_vec2 d)
{
	t_vec2	r;
	float	t;

	if (e->state == EN_DYING || e->state == EN_DEAD)
		return (-1.0f);
	r = (t_vec2){e->pos.x - pos.x, e->pos.y - pos.y};
	t = r.x * d.x + r.y * d.y;
	if (t <= 0.0f || fabsf(r.x * d.y - r.y * d.x) >= ENEMY_HIT_RADIUS)
		return (-1.0f);
	return (t);
}

/* Le tir touche le plus proche des ennemis vivants proches de l'axe */
int	horde_shoot(t_horde *h, const t_grid *g, t_vec2 pos, float angle)
{
	t_vec2	d;
	float	best_t;
	float	t;
	int		best;
	int		i;

	d = (t_vec2){cosf(angle), sinf(angle)};
	best_t = cast_ray(g, pos, d).dist;
	best = -1;
	i = -1;
	while (++i < h->n)
	{
		t = along_shot(&h->v[i], pos, d);
		if (t > 0.0f && t < best_t)
		{
			best_t = t;
			best = i;
		}
	}
	if (best < 0)
		return (-1);
	h->v[best].hp -= PISTOL_DAMAGE_MIN + horde_rand(h) % PISTOL_DAMAGE_RAND;
	h->v[best].state = EN_PAIN;
	if (h->v[best].hp <= 0)
		h->v[best].state = EN_DYING;
	h->v[best].t = 0.0;
	h->v[best].moving = false;
	return (best);
}

/* 0 de face, 4 de dos ; 6 quand on le voit tourné vers sa droite à l'écran */
static int	view_col(const t_enemy *e, t_vec2 viewer)
{
	float	rel;
	int		k;

	rel = atan2f(viewer.y - e->pos.y, viewer.x - e->pos.x) - e->facing;
	while (rel > PI_F)
		rel -= 2 * PI_F;
	while (rel < -PI_F)
		rel += 2 * PI_F;
	k = (int)lroundf(rel / (PI_F / 4));
	return (((-k) % 8 + 8) % 8);
}

static int	attack_col(double t)
{
	if (t < ENEMY_SHOT_1)
		return (0);
	if (t < ENEMY_SHOT_1 + 0.12)
		return (1);
	if (t < ENEMY_SHOT_2)
		return (2);
	if (t < ENEMY_SHOT_2 + 0.12)
		return (3);
	return (2);
}

t_cell	enemy_cell(const t_enemy *e, t_vec2 viewer)
{
	int	k;

	if (e->state == EN_ATTACK)
		return ((t_cell){attack_col(e->t), 6});
	if (e->state == EN_PAIN)
		return ((t_cell){0, 5});
	if (e->state == EN_DEAD)
		return ((t_cell){6, 5});
	if (e->state == EN_DYING)
	{
		k = (int)(e->t / ENEMY_DEATH_FRAME);
		if (k > 6)
			k = 6;
		return ((t_cell){k, 5});
	}
	if (e->moving)
		return ((t_cell){view_col(e, viewer), 1 + (int)(e->walk_t / 0.2) % 4});
	return ((t_cell){view_col(e, viewer), 0});
}
