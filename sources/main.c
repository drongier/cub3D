/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:45:28 by drongier          #+#    #+#             */
/*   Updated: 2025/03/21 11:51:36 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	ft_cleanup(t_scene *scene)
{
	int	i;

	i = 0;
	while (scene->lines && scene->lines[i])
		free(scene->lines[i++]);
	if (scene->lines)
		free(scene->lines);
	if (scene->no_texture)
		free(scene->no_texture);
	if (scene->so_texture)
		free(scene->so_texture);
	if (scene->we_texture)
		free(scene->we_texture);
	if (scene->ea_texture)
		free(scene->ea_texture);
	if (scene->f_color)
		free(scene->f_color);
	if (scene->c_color)
		free(scene->c_color);
	if (scene->map->coor)
		free(scene->map->coor);
	i = 0;
	while (scene->map->map && scene->map->map[i])
		free(scene->map->map[i++]);
	free(scene->map->map);
}

void	ft_error(t_scene *scene)
{
	printf("Error\n");
	ft_cleanup(scene);
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
	init_parser(&scene, &map);
	get_scene_data(argv[1], &scene);
	get_map(&map);
	init_game(&game, &scene, &map);
	mlx_hook(game.win, 2, 1L << 0, key_press, &game.player);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game.player);
	mlx_hook(game.win, 17, 0L, close_window, &game);
	draw_loop(&game);
	mlx_loop_hook(game.mlx, draw_loop, &game);
	mlx_loop(game.mlx);
	exit_game(&game);
	ft_cleanup(&scene);
	return (0);
}
