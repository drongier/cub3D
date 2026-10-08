/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:45:28 by drongier          #+#    #+#             */
/*   Updated: 2025/05/12 15:13:54 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static void	ft_cleanup_scene(t_scene *scene)
{
	int	i;

	i = 0;
	while (scene && scene->lines && scene->lines[i])
		free(scene->lines[i++]);
	if (scene && scene->lines)
		free(scene->lines);
	if (scene && scene->no_texture)
		free(scene->no_texture);
	if (scene && scene->so_texture)
		free(scene->so_texture);
	if (scene && scene->we_texture)
		free(scene->we_texture);
	if (scene && scene->ea_texture)
		free(scene->ea_texture);
	if (scene && scene->f_color)
		free(scene->f_color);
	if (scene && scene->c_color)
		free(scene->c_color);
}

static void	ft_cleanup_map(t_map *map)
{
	int	i;

	i = 0;
	while (map && map->map && map->map[i])
		free(map->map[i++]);
	if (map && map->coor)
		free(map->coor);
	if (map && map->map)
		free(map->map);
}

void	ft_error(t_scene *scene, char *message)
{
	printf("Error: %s\n", message);
	ft_cleanup_map(scene->map);
	ft_cleanup_scene(scene);
	exit(1);
}

void	game_destroy(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
		texture_free(&game->textures[i++]);
	free(game->fb);
	game->fb = NULL;
	minimap_free(&game->minimap);
	game->data = NULL;
	platform_destroy(game->platform);
	game->platform = NULL;
}

int	main(int argc, char **argv)
{
	t_game		game;
	t_scene		scene;
	t_map		map;
	t_options	opt;
	int			status;

	if (!parse_options(argc, argv, &opt))
		return (print_usage(), 1);
	ft_config_file_check((char *)opt.scene_path);
	init_parser(&scene, &map);
	get_scene_data((char *)opt.scene_path, &scene);
	ft_textures_files_check(&scene);
	get_map(&scene, &map);
	init_game(&game, &scene, &map);
	game.platform = platform_init(WIDTH, HEIGHT, opt.vsync);
	if (!game.platform)
	{
		game_destroy(&game);
		ft_error(&scene, "Window initialisation failed!");
	}
	ft_cleanup_scene(&scene);
	status = run_loop(&game, &opt);
	game_destroy(&game);
	ft_cleanup_map(&map);
	return (status);
}
