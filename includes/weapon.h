#ifndef WEAPON_H
# define WEAPON_H

# include <stdbool.h>
# include "motion.h"

/* Tir au coup par coup, au plus un tir toutes les WEAPON_COOLDOWN s */
# define WEAPON_COOLDOWN 0.25
# define WEAPON_FLASH_TIME 0.06
# define WEAPON_RECOIL_TIME 0.2
/* Déplacements de l'arme à l'écran, en pixels */
# define WEAPON_RECOIL_PX 22.0f
# define WEAPON_BOB_PX 12.0f
/* Balancement : cycles par seconde en marchant, plus vite en sprint */
# define WEAPON_BOB_HZ 1.6f
/* Vitesse à laquelle le balancement apparaît et s'éteint, par seconde */
# define WEAPON_BOB_EASE 6.0f
# define WEAPON_IDLE_XPM "textures/weapon/pistol_idle.xpm"
# define WEAPON_FIRE_XPM "textures/weapon/pistol_fire.xpm"
/* Les images font 48 x 48 pixels, affichées 5 fois plus grandes */
# define WEAPON_SCALE 5

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
bool	weapon_flash(const t_weapon *w);
/* Décalage de l'arme à l'écran (y vers le bas) : balancement + recul */
t_vec2	weapon_offset(const t_weapon *w);

#endif
