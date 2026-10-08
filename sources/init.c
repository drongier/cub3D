/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 11:39:19 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/09 16:27:18 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_parser(t_scene *scene, t_map *map)
{
	scene->row = 0;
	scene->lines = NULL;
	scene->no_texture = NULL;
	scene->so_texture = NULL;
	scene->we_texture = NULL;
	scene->ea_texture = NULL;
	scene->f_color = NULL;
	scene->c_color = NULL;
	scene->texture_count = 0;
	scene->color_count = 0;
	scene->map_first_line = 0;
	scene->map_last_line = 0;
	scene->map = map;
	map->row = 0;
	map->col = 0;
	map->player_x = 0;
	map->player_y = 0;
	map->player_o = 0;
	map->player_flag = 0;
	map->ceiling = 0;
	map->floor = 0;
	map->coor = NULL;
	map->map = NULL;
}

void	init_textures(t_game *game)
{
	char	*paths[4];
	int		i;

	paths[0] = game->scene->no_texture;
	paths[1] = game->scene->so_texture;
	paths[2] = game->scene->we_texture;
	paths[3] = game->scene->ea_texture;
	i = 0;
	while (i < 4)
	{
		if (!xpm_load(paths[i], &game->textures[i]))
		{
			game_destroy(game);
			ft_error(game->scene, "Issue about texture file!");
		}
		i++;
	}
}

void	init_player_angle(t_player *player, t_map *map)
{
	if (map->player_o == 'N')
		player->angle = 3 * PI / 2;
	else if (map->player_o == 'S')
		player->angle = PI / 2;
	else if (map->player_o == 'E')
		player->angle = 0;
	else if (map->player_o == 'W')
		player->angle = PI;
}

void	init_player(t_player *player, t_map *map, t_game *game)
{
	player->o = map->player_o;
	player->x = (float)map->player_x * BLOCK + BLOCK / 2;
	player->y = (float)map->player_y * BLOCK + BLOCK / 2 ;
	player->hit_dir = -1;
	player->hit_x = 0;
	player->hit_y = 0;
	player->key_up = false;
	player->key_down = false;
	player->key_right = false;
	player->key_left = false;
	player->key_exit = false;
	player->left_rotate = false;
	player->right_rotate = false;
	player->game = game;
	init_player_angle(player, map);
}

// initialisation functions
void	init_game(t_game *game, t_scene *scene, t_map *map)
{
	ft_bzero(game, sizeof(*game));
	game->map = map;
	game->scene = scene;
	game->player.map = map;
	init_player(&game->player, map, game);
	init_textures(game);
	game->fb = ft_calloc((size_t)WIDTH * HEIGHT, sizeof(uint32_t));
	if (!game->fb)
	{
		game_destroy(game);
		ft_error(scene, "Allocation error!");
	}
	game->data = (char *)game->fb;
	game->bpp = 32;
	game->size_line = WIDTH * (int)sizeof(uint32_t);
}
