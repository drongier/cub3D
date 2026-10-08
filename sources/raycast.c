#include "../includes/raycast.h"
#include <math.h>

/* Distance minimale : évite une hauteur infinie collé au mur */
#define MIN_DIST 1e-3f

t_camera	camera_make(t_vec2 pos, float angle, int screen_w)
{
	t_camera	cam;
	float		half;

	half = tanf(FOV_DEG / 2.0f * PI_F / 180.0f);
	cam.pos = pos;
	cam.dir = (t_vec2){cosf(angle), sinf(angle)};
	cam.plane = (t_vec2){-cam.dir.y * half, cam.dir.x * half};
	cam.focal = screen_w / 2.0f / half;
	return (cam);
}

/* Rayon passant par le centre de la colonne, de -plane à +plane */
t_vec2	camera_ray(const t_camera *cam, int column, int screen_w)
{
	float	x;

	x = 2.0f * (column + 0.5f) / screen_w - 1.0f;
	return ((t_vec2){cam->dir.x + cam->plane.x * x,
		cam->dir.y + cam->plane.y * x});
}

static float	delta(float d)
{
	if (d == 0.0f)
		return (1e30f);
	return (fabsf(1.0f / d));
}

/* Distances jusqu'à la première ligne verticale et horizontale de la grille */
static void	first_side(t_vec2 pos, t_vec2 ray, int cell[2], float side[2])
{
	if (ray.x < 0)
		side[0] = (pos.x - cell[0]) * delta(ray.x);
	else
		side[0] = (cell[0] + 1.0f - pos.x) * delta(ray.x);
	if (ray.y < 0)
		side[1] = (pos.y - cell[1]) * delta(ray.y);
	else
		side[1] = (cell[1] + 1.0f - pos.y) * delta(ray.y);
}

static t_hit	make_hit(t_vec2 pos, t_vec2 ray, int y_side, float dist)
{
	t_hit	h;

	h.dist = dist;
	h.point = (t_vec2){pos.x + ray.x * dist, pos.y + ray.y * dist};
	if (!y_side)
	{
		h.wall_x = h.point.y - floorf(h.point.y);
		h.face = FACE_EAST;
		if (ray.x < 0)
			h.face = FACE_WEST;
	}
	else
	{
		h.wall_x = h.point.x - floorf(h.point.x);
		h.face = FACE_SOUTH;
		if (ray.y < 0)
			h.face = FACE_NORTH;
	}
	if ((h.face == FACE_WEST || h.face == FACE_SOUTH) && h.wall_x > 0.0f)
		h.wall_x = 1.0f - h.wall_x;
	if (h.dist < MIN_DIST)
		h.dist = MIN_DIST;
	return (h);
}

/*
 * DDA : on avance de ligne de grille en ligne de grille. ray n'a pas besoin
 * d'être normé : avec un rayon issu de camera_ray, la distance renvoyée est
 * directement la distance au plan caméra, sans effet fisheye.
 */
t_hit	cast_ray(const t_grid *g, t_vec2 pos, t_vec2 ray)
{
	int		cell[2];
	float	side[2];
	int		y_side;

	cell[0] = (int)floorf(pos.x);
	cell[1] = (int)floorf(pos.y);
	first_side(pos, ray, cell, side);
	y_side = 0;
	while (1)
	{
		y_side = side[1] < side[0];
		if (!y_side)
		{
			side[0] += delta(ray.x);
			cell[0] += 1 - 2 * (ray.x < 0);
		}
		else
		{
			side[1] += delta(ray.y);
			cell[1] += 1 - 2 * (ray.y < 0);
		}
		if (grid_at(g, cell[0], cell[1]) == CELL_WALL)
			break ;
	}
	return (make_hit(pos, ray, y_side, side[y_side] - delta(y_side ? ray.y
				: ray.x)));
}

int	wall_height(const t_camera *cam, float dist)
{
	if (dist < MIN_DIST)
		dist = MIN_DIST;
	return ((int)(cam->focal / dist));
}
