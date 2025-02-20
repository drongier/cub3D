/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:43:05 by drongier          #+#    #+#             */
/*   Updated: 2025/02/19 16:19:13 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

# define WIDTH 1280
# define HEIGHT 720
# define BLOCK 64
# define DEBUG 0

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363
# define EXIT 65307

# define PI 3.14159265359

#include "../minilibx-linux/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct s_player
{
    float x;
    float y;
    float angle;

    bool key_up;
    bool key_down;
    bool key_left;
    bool key_right;
	bool key_exit;

    bool left_rotate;
    bool right_rotate;
	struct s_game *game;
	
}   t_player;

typedef struct s_game
{
    void *mlx;
    void *win;
    void *img;

    char *data;
    int bpp;
    int size_line;
    int endian;
    t_player player;

    char **map;
} t_game;


// INITIALISATION

void	init_player(t_player *player);

// PLAYER MOVEMENT

int		key_release(int keycode, t_player *player);
int		key_press(int keycode, t_player *player);
void	move_player(t_player *player);

// UTILS 
int		close_window(t_game *game);
void	clear_image(t_game *game);

// DRAWING FUNCTIONS
int		draw_loop(t_game *game);
bool	touch(float px, float py, t_game *game);
float	fixed_dist(float x1, float y1, float x2, float y2, t_game *game);
void	put_pixel(int x, int y, int color, t_game *game);

// DEBUG OR BONUS MINIMAP

void draw_map(t_game *game);
void draw_square(int x, int y, int size, int color, t_game *game);

#endif