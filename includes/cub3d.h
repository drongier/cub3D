/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:43:05 by drongier          #+#    #+#             */
/*   Updated: 2025/03/11 15:03:50 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define WIDTH 1280
# define HEIGHT 720
# define BLOCK 64
# define BONUS 0

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363
# define EXIT 65307

# define PI 3.14159265359

# define NORD 0
# define SUD  1
# define WEST 2
# define EAST 3

# include "../minilibx-linux/mlx.h"
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>

typedef struct s_player
{
    float	x;
    float	y;
    float	angle;

    bool	key_up;
    bool	key_down;
    bool	key_left;
    bool	key_right;
	bool	key_exit;
    bool	left_rotate;
    bool	right_rotate;
	int		hit_dir;
	struct s_game *game;
	
}   t_player;

typedef struct s_game
{
    void	*mlx;
    void	*win;
    void	*img;

    char *data;
    int bpp;
    int size_line;
    int endian;
    t_player player;

    char **map;
	int map_width;
	int map_height;
} t_game;


//EXIT

void	exit_game(t_game *game);

// INITIALISATION

void	init_player(t_player *player);

// PLAYER MOVEMENT

int		key_release(int keycode, t_player *player);
int		key_press(int keycode, t_player *player);
void	move_player(t_player *player);
void	m_up(t_player *player, float cos_angle, float sin_angle, int speed);
void	m_down(t_player *player, float cos_angle, float sin_angle, int speed);
void	m_left(t_player *player, float cos_angle, float sin_angle, int speed);
void	m_right(t_player *player, float cos_angle, float sin_angle, int speed);
void	update_angle(t_player *player, float angle_speed);
void	check_boundaries(t_player *player);


// UTILS

int		close_window(t_game *game);
void	clear_image(t_game *game);
void	get_size_map(t_game *game);
int		is_nord_sud(float start_x);
int		is_west_east(float start_x);
int 	get_hit_direction(float start_x, int is_vertical);

// UTILS MATH 

float	fixed_dist(float x1, float y1, float x2, float y2, t_game *game);
float	distance(float x, float y);
int		calc_ray(t_player *player, float start_x, float *ray_x, float *ray_y);
float	calculate_height(t_player *player, float ray_x, float ray_y);

// DRAWING FUNCTIONS

int		draw_loop(t_game *game);
bool	touch(float px, float py, t_game *game);
void	put_pixel(int x, int y, int color, t_game *game);
void	draw_ground(int i, int start_y, t_game *game);
void	draw_wall(int i, int *start_y, int end, t_game *game);
void	draw_ceiling(int i, int start_y, t_game *game);

// DEBUG OR BONUS MINIMAP

void	draw_map(t_game *game, int offset_x, int offset_y);
void	draw_square_full(int x, int y, int size, int color, t_game *game);
void	draw_mini_map(t_game *game);
void	draw_scope(t_game *game);
void	wall_colision(t_player *player);
void	check_collision(t_player *player, float dx, float dy);

#endif