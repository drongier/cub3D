#ifndef LEVEL_H
# define LEVEL_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

# define LEVEL_MAX_SIDE 1000
# define LEVEL_MAX_FILE 16777216
# define LEVEL_MAX_ENEMIES 256
# define LEVEL_MAX_SPAWNS 64
# define LEVEL_MAX_ITEMS 32

/* Même ordre que les faces du raycaster */
enum e_level_tex
{
	TEX_NO,
	TEX_SO,
	TEX_WE,
	TEX_EA
};

/*
 * Case d'un ennemi ('M' dans la map), d'un point d'apparition des vagues
 * ('X') ou d'une trousse de soins ('H'), et sa ligne dans le fichier
 */
typedef struct s_level_enemy
{
	int	x;
	int	y;
	int	line;
}	t_level_enemy;

/*
 * Scène chargée. cells : w x h caractères, '1' mur, '0' sol (spawn,
 * ennemis, points d'apparition et trousses compris), ' ' vide. spawn_dir : 'N', 'S',
 * 'E' ou 'W'. Au moins un point d'apparition : la scène se joue en vagues.
 */
typedef struct s_level
{
	char		*tex[4];
	uint32_t	floor;
	uint32_t	ceiling;
	int			w;
	int			h;
	char		*cells;
	int			spawn_x;
	int			spawn_y;
	char		spawn_dir;
	t_level_enemy	*enemies;
	int			n_enemies;
	t_level_enemy	*spawns;
	int			n_spawns;
	t_level_enemy	*items;
	int			n_items;
}	t_level;

/* line : ligne du fichier en cause (à partir de 1), 0 si aucune */
typedef struct s_level_error
{
	int		line;
	char	msg[192];
}	t_level_error;

/* Rien n'est alloué dans out quand ces fonctions renvoient false */
bool	level_parse(const char *text, size_t len, t_level *out,
			t_level_error *err);
bool	level_load(const char *path, t_level *out, t_level_error *err);
void	level_free(t_level *lv);
void	level_print_error(const t_level_error *err);

#endif
