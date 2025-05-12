/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:07:44 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/12 15:01:55 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	map_row_col(t_scene *scene)
{
	int		i;
	int		len;
	t_map	*map;

	len = 0;
	map = scene->map;
	map->row = scene->map_last_line - scene->map_first_line + 1;
	i = scene->map_first_line;
	while (i < scene->map_last_line)
	{
		len = ft_strlen(scene->lines[i]) - 1;
		if (len > map->col)
			map->col = len;
		i++;
	}
}

void	get_map(t_scene *scene, t_map *map)
{
	int	i;
	int	j;

	map->map = (char **)ft_calloc(map->row + 1, sizeof(char *));
	if (!map->map)
		ft_error(scene, "Allocation error!");
	i = 0;
	while (i < map->row)
	{
		map->map[i] = (char *)ft_calloc(map->col + 1, sizeof(char));
		if (!map->map[i])
			ft_error(scene, "Allocation error!");
		j = 0;
		while (j < map->col)
		{
			map->map[i][j] = map->coor[i * map->col + j] + 48;
			j++;
		}
		map->map[i][j] = 0;
		i++;
	}
	map->map[i] = NULL;
}

void	parse_map(t_scene *scene)
{
	t_map	*map;
	int		tmp;

	map = scene->map;
	map_row_col(scene);
	map->coor = (char *)ft_calloc(map->row * map->col, sizeof(char));
	if (!map->coor)
		ft_error(scene, "Allocation error!");
	extract_map(scene, map);
	if (!map->player_o)
		ft_error(scene, "Missing spawning position / Wrong direction value");
	enclosed_map_check(scene, map);
	encode_ceiling_color(scene, map);
	encode_floor_color(scene, map);
	tmp = scene->map->player_x;
	scene->map->player_x = scene->map->player_y;
	scene->map->player_y = tmp;
}
