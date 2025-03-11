/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flood_fill.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:04:57 by mekundur          #+#    #+#             */
/*   Updated: 2025/03/11 16:27:51 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void    ft_flood_fill(t_scene *scene, bool *visited, int x, int y)
{
    t_map   *map;

    map = scene->map;
    if (x < 0 || x >= map->row || y < 0 || y >= map->col)
    {
        free(visited);
        ft_error(scene);
    }
    if (map->coor[x * map->col + y] == 1 || visited[x * map->col + y])
        return;
    else if(x >= 0 && x < map->row && y >= 0 && y < map->col && map->coor[x * map->col + y] == 0)
        visited[x * map->col + y] = 1;
    // printf("%d %d \n", x, y);
    ft_flood_fill(scene, visited, x, y + 1);
    ft_flood_fill(scene, visited, x, y - 1);
    ft_flood_fill(scene, visited, x + 1, y);
    ft_flood_fill(scene, visited, x - 1, y);
    ft_flood_fill(scene, visited, x + 1, y + 1);
    ft_flood_fill(scene, visited, x + 1, y - 1);
    ft_flood_fill(scene, visited, x - 1, y + 1);
    ft_flood_fill(scene, visited, x - 1, y - 1);
}

void    enclosed_map_check(t_scene *scene, t_map *map)
{
    bool    *visited;

    visited = (bool *)ft_calloc(map->row * map->col, sizeof(bool));
    ft_flood_fill(scene, visited, map->player_x, map->player_y);
    free(visited);

	int i, j;
    i = 0;
	while (i < map->row)
	{
		j = 0;
		while(j < map->col)
		{
			printf("%d", map->coor[i * map->col + j]);
			j++;
		}
		printf("\n");
		i++;
	}	
    printf("\n");
}