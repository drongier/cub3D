#ifndef WEAPON_H
# define WEAPON_H

# include <stdbool.h>
# include "motion.h"

/* Tir au coup par coup, au plus un tir toutes les WEAPON_COOLDOWN s */
# define WEAPON_COOLDOWN 0.25
/*
 * Animation d'un tir, 5 images : 0 repos, puis 1 arme levée, 2 flash,
 * 3 recul, 4 retour. WEAPON_ANIM_n : fin de l'image n, en s après le tir.
 */
# define WEAPON_FRAMES 5
# define WEAPON_ANIM_1 0.04
# define WEAPON_ANIM_2 0.10
# define WEAPON_ANIM_3 0.18
# define WEAPON_ANIM_4 0.25
/* Amplitude du balancement à l'écran, en pixels */
# define WEAPON_BOB_PX 12.0f
/* Balancement : cycles par seconde en marchant, plus vite en sprint */
# define WEAPON_BOB_HZ 1.6f
/* Vitesse à laquelle le balancement apparaît et s'éteint, par seconde */
# define WEAPON_BOB_EASE 6.0f
/*
 * Images WEAPON_XPM_PREFIX_0.xpm à _4.xpm, 64 x 64. Comme dans Wolfenstein
 * 3D, la case entière fait à peu près la hauteur de l'écran : 64 x 11 = 704.
 */
# define WEAPON_XPM_PREFIX "textures/weapon/pistol"
# define WEAPON_SCALE 11

typedef struct s_weapon
{
	double	since_shot;
	bool	trigger_was_down;
	float	bob_phase;
	float	bob_amount;
}	t_weapon;

void	weapon_init(t_weapon *w);
/* Renvoie true quand un tir part pendant cette frame */
bool	weapon_update(t_weapon *w, bool trigger, t_move move, double dt);
/* Image de l'animation à afficher, de 0 à WEAPON_FRAMES - 1 */
int		weapon_frame(const t_weapon *w);
/* Balancement de l'arme à l'écran (y vers le bas) */
t_vec2	weapon_offset(const t_weapon *w);

#endif
