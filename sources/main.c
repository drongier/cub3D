/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:45:28 by drongier          #+#    #+#             */
/*   Updated: 2025/03/11 16:55:34 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// initialisation functions
// char	**get_map(void)
// {
//     char **map = malloc(sizeof(char *) * 11);
//     map[0] = "111111111111111";
//     map[1] = "100000100000001";
//     map[2] = "100000100000001";
//     map[3] = "100000100000101";
//     map[4] = "100000000001001";
//     map[5] = "100000010000001";
//     map[6] = "1000000000P0001";
//     map[7] = "101010101000001";
//     map[8] = "100000000000001";
//     map[9] = "111111111111111";
//     map[10] = NULL;
//     return (map);
// }

void	init_game(t_game *game)
{
	init_player(&game->player);
	game->player.game = game;
	get_size_map(game);
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

void	free_map(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
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
	}
	free(game->mlx);
	free_map(game->map);
}

void	ft_cleanup(t_scene *scene)
{
	t_map	*map;
	int		i;

	map = scene->map;
	i = 0;
	while (scene->lines && scene->lines[i])
		free(scene->lines[i++]);
	free(scene->lines);
	free(scene->no_texture);
	free(scene->so_texture);
	free(scene->we_texture);
	free(scene->ea_texture);
	free(scene->f_color);
	free(scene->c_color);
	free(scene->map->coor);
	i = 0;
	while(map->map && map->map[i])
		free(map->map[i++]);
	free(map->map);
}	

void	ft_error(t_scene *scene)
{
	printf("Error\n");
	ft_cleanup(scene);
	exit(1);
}

void	initialize(t_scene *scene, t_map *map)
{
	scene->col = 0;
	scene->row = 0;
	scene->lines = NULL;  
	scene->no_texture = NULL;
	scene->so_texture = NULL;
	scene->we_texture = NULL;
	scene->ea_texture = NULL;
	scene->f_color = NULL;
	scene->c_color = NULL;
	scene->del_line = 0;
	scene->map_first_line = 0;
	scene->map_last_line = 0;
	scene->map = map;
	map->row = 0;
	map->col = 0;
	map->player_x = 0;
	map->player_y = 0;
	map->player_o = 0;
	map->player_flag = 0;
	map->coor = NULL;
}

int	main(int argc, char **argv)
{
	t_game	game;
	t_scene	scene;
	t_map	map;

	if (argc != 2)
	{
		printf("Please execute the program as: ./cub3D <scene.cub>\n");
		return (0);
	}
	initialize(&scene, &map);
	get_scene_data(argv[1], &scene);
	get_map(&game, &map);
	// game->map = map->map;
	init_game(&game);
	mlx_hook(game.win, 2, 1L << 0, key_press, &game.player);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game.player);
	mlx_hook(game.win, 17, 0L, close_window, &game);
	mlx_loop_hook(game.mlx, draw_loop, &game);
	mlx_loop(game.mlx);
	exit_game(&game);
	
	// while (scene.lines && *scene.lines)
	// 	printf("%s", *(scene.lines++));
	// printf("%s", scene.no_texture);
	// printf("%s", scene.so_texture);
	// printf("%s", scene.we_texture);
	// printf("%s", scene.ea_texture);
	// printf("%s", scene.f_color);
	// printf("%s", scene.c_color);
	// printf("%d\n", scene.row);
	// printf("%d\n", scene.map_first_line);
	// printf("%d\n", scene.map_last_line);
//	while (map.map && *map.map)
//		printf("%s", *(map.map++));

	ft_cleanup(&scene);
	return (0);
}