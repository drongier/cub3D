/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flood_fill.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:04:57 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/09 15:32:15 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	ft_flood_fill(t_scene *scene, bool *visited, int x, int y)
{
	t_map	*map;

	map = scene->map;
	if (x < 0 || x >= map->row || y < 0 || y >= map->col)
	{
		// printf("x %d row %d y %d col %d\n", x, map->row, y, map->col);
		free(visited);
		ft_error(scene, "Open map borders or player is borderline! :s");
	}
	if (map->coor[x * map->col + y] == 1 || visited[x * map->col + y])
		return ;
	else if (x >= 0 && x < map->row && y >= 0
		&& y < map->col && map->coor[x * map->col + y] == 0)
		visited[x * map->col + y] = 1;
	ft_flood_fill(scene, visited, x, y + 1);
	ft_flood_fill(scene, visited, x, y - 1);
	ft_flood_fill(scene, visited, x + 1, y);
	ft_flood_fill(scene, visited, x - 1, y);
	ft_flood_fill(scene, visited, x + 1, y + 1);
	ft_flood_fill(scene, visited, x + 1, y - 1);
	ft_flood_fill(scene, visited, x - 1, y + 1);
	ft_flood_fill(scene, visited, x - 1, y - 1);
}

void	enclosed_map_check(t_scene *scene, t_map *map)
{
	bool	*visited;

	visited = (bool *)ft_calloc(map->row * map->col, sizeof(bool));
	if (!visited)
		ft_cleanup(scene);
	
	// printf("map_first_line %d\n", scene->map_first_line);
	// printf("map_last_line %d\n", scene->map_last_line);
	// printf("scene_row %d\n", scene->row);
	// printf("map_row %d, map_col %d\n", scene->map->row, scene->map->col);
	
	ft_flood_fill(scene, visited, map->player_x, map->player_y);
	free(visited);
}
