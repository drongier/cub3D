/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:43:05 by drongier          #+#    #+#             */
/*   Updated: 2025/05/12 13:57:54 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define WIDTH 1280
# define HEIGHT 720
# define BLOCK 64
# define BONUS 1
# define MM_SIZE 200
# define MM_MARGIN 16
# define MM_CELL 12
# define MM_CONE_STEP 8
# define MM_FLOOR_RGB 0x000000
# define MM_FLOOR_A 140
# define MM_WALL_RGB 0xC8C8C8
# define MM_WALL_A 230
# define MM_VOID_RGB 0x000000
# define MM_VOID_A 210
# define MM_CONE_RGB 0xFFE070
# define MM_CONE_A 90
# define MM_PLAYER_RGB 0x40FF40
# define MM_ENEMY_RGB 0xFF3030
# define MM_ITEM_RGB 0x40C0FF
# define MM_FRAME_RGB 0xFFFFFF
# define MM_FRAME_A 160
# define COLLISION_MARG 10
# define PLAYER_MAX_HP 100
/* Rayon du joueur face aux ennemis, en cases */
# define PLAYER_RADIUS 0.25f
/* Durée du flash rouge quand le joueur est touché */
# define HURT_FLASH_TIME 0.35
/* Durée du flash vert quand on ramasse une trousse */
# define HEAL_FLASH_TIME 0.35
/* Délai avant de pouvoir recommencer après la mort */
# define RESTART_DELAY 1.0
# define MUTANT_XPM "textures/enemies/mutant.xpm"

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363
# define EXIT 65307

# define PI 3.14159265359

# include "../libft/libft.h"
# include "platform.h"
# include "texture.h"
# include "options.h"
# include "stats.h"
# include "motion.h"
# include "raster.h"
# include "raycast.h"
# include "pixels.h"
# include "level.h"
# include "weapon.h"
# include "enemy.h"
# include "waves.h"
# include "items.h"
# include "font.h"
# include <stdint.h>
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_player
{
	float			x;
	float			y;
	float			angle;
	bool			key_up;
	bool			key_down;
	bool			key_left;
	bool			key_right;
	bool			left_rotate;
	bool			right_rotate;
	bool			sprint;
	struct s_game	*game;
}					t_player;

typedef struct s_game
{
	t_platform		*platform;
	uint32_t		*fb;
	t_texture		textures[4];
	t_player		player;
	t_level			level;
	t_grid			grid;
	t_vec2			hits[WIDTH];
	t_weapon		weapon;
	t_texture		weapon_tex[WEAPON_FRAMES];
	bool			trigger;
	bool			trigger_was_down;
	float			zbuf[WIDTH];
	t_horde			horde;
	t_texture		mutant_tex;
	const char		*level_path;
	int				hp;
	double			hurt_t;
	double			dead_t;
	t_waves			waves;
	int				kills;
	t_items			items;
	t_texture		medkit_tex;
	double			heal_t;
}					t_game;

/* Un point du radar : position en cases et couleur */
typedef struct s_dot
{
	t_vec2			pos;
	uint32_t		color;
}					t_dot;

// GAME

bool				game_init(t_game *game, t_level_error *err);
bool				game_load_level(t_game *game, const char *path,
						t_level_error *err);
void				game_unload_level(t_game *game);
void				game_destroy(t_game *game);
/* Une frame de jeu ; false si la partie ne peut pas continuer */
bool				game_update(t_game *game, double dt);

// PLAYER MOVEMENT

void				update_player(t_player *player, double dt);
t_move				player_move(const t_player *player);

// MAIN LOOP

int					run_loop(t_game *game, const t_options *opt);

// DRAWING FUNCTIONS

int					draw_loop(t_game *game);
bool				touch(float px, float py, t_game *game);
bool				player_blocked(t_game *game, float px, float py);
void				draw_sprites(t_game *game, const t_camera *cam);
void				draw_hud(t_game *game);

// BONUS MINIMAP

void				draw_minimap(t_game *game);
void				draw_weapon(t_game *game);
void				draw_scope(t_game *game);

#endif
