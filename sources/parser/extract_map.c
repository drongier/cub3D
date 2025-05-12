/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:18:01 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/12 13:10:02 by mekundur         ###   ########.fr       */
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
		ft_error(scene, "Multiple spawning points!");
	map->player_flag++;
	map->player_x = i;
	map->player_y = j;
	map->player_o = scene->lines[line][j];
}

int	extract_map_inner_loop(t_scene *scene, int line, int i)
{
	int		j;
	t_map	*map;

	map = scene->map;
	j = 0;
	while (scene->lines[line][j] && scene->lines[line][j] != '\n'
		&& j < scene->map->col)
	{
		if (ft_is_whitespace(scene->lines[line][j]))
			map->coor[i * map->col + j] = 0;
		else if (scene->lines[line][j] == '0')
			map->coor[i * map->col + j] = 0;
		else if (scene->lines[line][j] == '1')
			map->coor[i * map->col + j] = 1;
		else if (ft_is_orientation(scene->lines[line][j]))
			extract_orientation_coor(scene, line, i, j);
		else
			ft_error(scene, "Broken or polluted map!");
		j++;
	}
	return (j);
}

void	extract_map(t_scene *scene, t_map *map)
{
	int	i;
	int	j;
	int	line;

	i = 0;
	line = scene->map_first_line;
	while (line < scene->map_last_line)
	{
		j = extract_map_inner_loop(scene, line, i);
		while (j < map->col)
		{
			map->coor[i * map->col + j] = 0;
			j++;
		}
		i++;
		line++;
	}
}
