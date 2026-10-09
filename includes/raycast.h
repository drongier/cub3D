#ifndef RAYCAST_H
# define RAYCAST_H

# include "grid.h"
# include "motion.h"

/* Champ de vision horizontal, en degrés */
# define FOV_DEG 66.0f

/* Indice de texture de la face touchée */
enum e_face
{
	FACE_NORTH,
	FACE_SOUTH,
	FACE_WEST,
	FACE_EAST
};

/*
 * dist : distance perpendiculaire au plan caméra, en cases.
 * wall_x : position sur le mur dans [0, 1), croissante de gauche à droite
 * à l'écran sur toutes les faces. point : impact, en cases.
 */
typedef struct s_hit
{
	float	dist;
	float	wall_x;
	int		face;
	t_vec2	point;
}	t_hit;

/* Direction du regard et plan caméra (perpendiculaire, longueur tan(FOV/2)) */
typedef struct s_camera
{
	t_vec2	pos;
	t_vec2	dir;
	t_vec2	plane;
	float	focal;
}	t_camera;

/* Point du monde vu par la caméra : profondeur, colonne, taille d'un bloc */
typedef struct s_proj
{
	float	depth;
	float	screen_x;
	int		size;
}	t_proj;

t_camera	camera_make(t_vec2 pos, float angle, int screen_w);
/* false si le point est derrière la caméra (ou presque collé) */
bool		camera_project(const t_camera *cam, t_vec2 p, int screen_w,
				t_proj *out);
t_vec2		camera_ray(const t_camera *cam, int column, int screen_w);
t_hit		cast_ray(const t_grid *g, t_vec2 pos, t_vec2 ray);
int			wall_height(const t_camera *cam, float dist);

#endif
