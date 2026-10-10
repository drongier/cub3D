#ifndef ITEMS_H
# define ITEMS_H

# include <stdbool.h>
# include "level.h"
# include "motion.h"

/* Trousse de soins : vie rendue, rayon de ramassage en cases */
# define MEDKIT_HEAL 25
# define MEDKIT_REACH 0.5f
/* Une trousse ramassée revient au bout de tant de secondes */
# define MEDKIT_RESPAWN 30.0
# define MEDKIT_XPM "textures/items/medkit.xpm"

/* t : secondes avant le retour d'une trousse ramassée */
typedef struct s_item
{
	t_vec2	pos;
	bool	present;
	double	t;
}	t_item;

typedef struct s_items
{
	t_item	*v;
	int		n;
}	t_items;

/* Une trousse sur chaque 'H' de la scène */
bool	items_init(t_items *it, const t_level *lv);
void	items_free(t_items *it);
/* Fait revenir les trousses dont le délai est écoulé */
void	items_update(t_items *it, double dt);
/* Toutes les trousses reviennent tout de suite (fin de vague) */
void	items_restock(t_items *it);
/*
 * Le joueur en p, avec hp sur max_hp, ramasse la trousse qu'il touche s'il
 * est blessé ; renvoie la vie rendue, 0 s'il n'a rien ramassé.
 */
int		items_pickup(t_items *it, t_vec2 p, int hp, int max_hp);

#endif
