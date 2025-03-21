/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:43:05 by drongier          #+#    #+#             */
/*   Updated: 2025/03/21 11:43:02 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define WIDTH 1280
# define HEIGHT 720
# define BLOCK 64
# define BONUS 1
# define MM_SIZE 8
# define COLLISION_MARG 10

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363
# define EXIT 65307

# define PI 3.14159265359

# define NORD 0
# define SUD 1
# define WEST 2
# define EAST 3

# include "../libft/libft.h"
# include "../minilibx-linux/mlx.h"
# include <bits/types.h>
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_map_data
{
	int				row;
	int				col;
	char			*coor;
	int				player_x;
	int				player_y;
	char			player_o;
	int				player_flag;
	char			**map;
	int				ceiling;
	int				floor;
}					t_map;

typedef struct s_scene_data
{
	int				row;
	char			**lines;
	char			*no_texture;
	char			*so_texture;
	char			*we_texture;
	char			*ea_texture;
	char			*f_color;
	char			*c_color;
	int				del_line;
	int				map_first_line;
	int				map_last_line;
	t_map			*map;
}					t_scene;

typedef struct s_player
{
	float			x;
	float			y;
	float			angle;
	char			o;
	bool			key_up;
	bool			key_down;
	bool			key_left;
	bool			key_right;
	bool			key_exit;
	bool			left_rotate;
	bool			right_rotate;
	int				hit_dir;
	float			hit_x;
	float			hit_y;
	struct s_game	*game;
	t_map			*map;
}					t_player;

typedef struct s_texture
{
	void			*img;
	char			*data;
	int				width;
	int				height;
	int				bpp;
	int				size_line;
	int				endian;
}					t_texture;

typedef struct s_minimap
{
	int				bottom_right_x;
	int				bottom_right_y;
	int				square_size;
}					t_minimap;

typedef struct s_game
{
	void			*mlx;
	void			*win;
	void			*img;
	char			*data;
	int				bpp;
	int				size_line;
	int				endian;
	char			*tex_no;
	int				tex_no_width;
	int				tex_no_height;
	char			*tex_so;
	char			*tex_ea;
	char			*tex_we;
	t_texture		textures[4];
	int				map_width;
	int				map_height;
	t_player		player;
	t_map			*map;
	t_scene			*scene;
	t_minimap		*minimap;
}					t_game;

// PARSER

void				get_colors(t_scene *scene);
void				get_textures(t_scene *scene);
void				get_scene_data(char *argv, t_scene *scene);
void				parse_map(t_scene *scene);
void				extract_map(t_scene *scene, t_map *map);
int					ft_row_count(char *argv);
void				ft_2dstrfree(char **str);
void				ft_error(t_scene *scene);
void				enclosed_map_check(t_scene *scene, t_map *map);
void				get_map(t_map *map);
void				encode_ceiling_color(t_scene *scene, t_map *map);
void				encode_floor_color(t_scene *scene, t_map *map);

// EXIT

void				exit_game(t_game *game);

// INITIALISATION

void				init_parser(t_scene *scene, t_map *map);
void				init_textures(t_game *game);
void				init_player(t_player *player, t_map *map, t_game *game);
void				init_game(t_game *game, t_scene *scene, t_map *map);

// PLAYER MOVEMENT

int					key_release(int keycode, t_player *player);
int					key_press(int keycode, t_player *player);
void				move_player(t_player *player);
void				m_up(t_player *player, float cos_angle, float sin_angle,
						int speed);
void				m_down(t_player *player, float cos_angle, float sin_angle,
						int speed);
void				m_left(t_player *player, float cos_angle, float sin_angle,
						int speed);
void				m_right(t_player *player, float cos_angle, float sin_angle,
						int speed);
void				update_angle(t_player *player, float angle_speed);
void				check_boundaries(t_player *player);
void				update_angle(t_player *player, float angle_speed);

// UTILS

int					close_window(t_game *game);
int					is_nord_sud(float start_x);
int					is_west_east(float start_x);

// UTILS MATH

float				distance(float x, float y);
void				calc_ray(t_player *player, float start_x, float *ray_x,
						float *ray_y);
float				calculate_height(t_player *player, float ray_x,
						float ray_y);

// DRAWING FUNCTIONS

int					draw_loop(t_game *game);
bool				touch(float px, float py, t_game *game);
void				put_pixel(int x, int y, int color, t_game *game);
void				draw_ground(int i, int start_y, t_game *game);
void				draw_wall(int i, int *start_y, int end, t_game *game);
void				draw_ceiling(int i, int start_y, t_game *game);

// BONUS MINIMAP

void				draw_mini_map(t_game *game);
void				draw_scope(t_game *game);
void				put_pixel_minimap(t_game *game, float x, float y,
						t_minimap *mini);
void				calcu_ray(float *x, float *y, float cos_a, float sin_a);
void				ray_tracing(t_game *game, float angle, float *x, float *y);

#endif
