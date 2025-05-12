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

void	exit_game(t_game *game)
{
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_loop_end(game->mlx);
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
}

int	main(int argc, char **argv)
{
	t_game	game;
	t_scene	scene;
	t_map	map;

	if (argc != 2)
		return (printf("Execute the program as: ./cub3D <scene.cub>\n"), 0);
	ft_config_file_check(argv[1]);
	init_parser(&scene, &map);
	get_scene_data(argv[1], &scene);
	ft_textures_files_check(&scene);
	get_map(&scene, &map);
	init_game(&game, &scene, &map);
	ft_cleanup_scene(&scene);
	mlx_hook(game.win, 2, 1L << 0, key_press, &game.player);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game.player);
	mlx_hook(game.win, 17, 0L, close_window, &game);
	draw_loop(&game);
	mlx_loop_hook(game.mlx, draw_loop, &game);
	mlx_loop(game.mlx);
	exit_game(&game);
	ft_cleanup_map(&map);
	return (0);
}
