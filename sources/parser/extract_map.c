/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:18:01 by mekundur          #+#    #+#             */
/*   Updated: 2025/03/20 18:51:46 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	ft_is_orientation(char s)
{
	char	*directions;

	directions = "WENS";
	if (ft_strchr(directions, s))
		return (1);
	return (0);
}

void	extract_orientation_coor(t_scene *scene, int line, int i, int j)
{
	t_map	*map;

	map = scene->map;
	if (map->player_flag)
		ft_error(scene);
	map->player_flag++;
	map->player_x = i;
	map->player_y = j;
	map->player_o = scene->lines[line][j];
}

void	extract_map(t_scene *scene, t_map *map)
{
	int	i;
	int	j;
	int	k;
	int	line;

	i = 0;
	line = scene->map_first_line;
	while (line < scene->map_first_line + map->row)
	{
		j = 0;
		k = 0;
		while (scene->lines[line][j] != '\n')
		{
			if (scene->lines[line][j] == '\t')
			{
				k = 0;
				while (k < 7)
				{
					map->coor[i * map->col + k] = 0;
					k++;
				}
			}
			else if (ft_is_whitespace(scene->lines[line][j]))
				map->coor[i * map->col + j + k] = 0;
			else if (scene->lines[line][j] == '0')
				map->coor[i * map->col + j + k] = 0;
			else if (scene->lines[line][j] == '1')
				map->coor[i * map->col + j + k] = 1;
			else if (ft_is_orientation(scene->lines[line][j]))
				extract_orientation_coor(scene, line, i, j);
			else
				ft_error(scene);
			j++;
		}
		while (j < map->col)
		{
			map->coor[i * map->col + j + k] = 0;
			j++;
		}
		i++;
		line++;
	}
}
