/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_colors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:29:08 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/08 19:15:40 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	encode_ceiling_color(t_scene *scene, t_map *map)
{
	char	**tmp;
	int		i;
	int		num;

	printf("%s\n", scene->c_color);
	tmp = ft_split(scene->c_color, ',');
	i = 0;
	while (tmp[i])
	{
		printf("i:%d %p %c\n", i, tmp[i], tmp[i][0]);
		num = ft_atoi(tmp[i]);
		printf("%s %d\n", tmp[i], num);
		if (!(num >= 0 && num <= 255) || (i < 3 && (!tmp[i][0] || tmp[i][0] == '\n')))
		{
			ft_2dstrfree(tmp);
			ft_error(scene, "Wrong color format for ceiling!");
		}
		map->ceiling = map->ceiling << 8;
		map->ceiling += num;
		i++;
	}
	ft_2dstrfree(tmp);
	if (i != 3)
		ft_error(scene, "Missing/redundant color channel for ceiling!");
}

void	encode_floor_color(t_scene *scene, t_map *map)
{
	char	**tmp;
	int		i;
	int		num;

	i = 0;
	tmp = ft_split(scene->f_color, ',');
	while (tmp[i])
	{
		num = ft_atoi(tmp[i]);
		if (!(num >= 0 && num <= 255))
		{
			ft_2dstrfree(tmp);
			ft_error(scene, "Wrong color format for floor!");
		}
		map->floor = map->floor << 8;
		map->floor += num;
		i++;
	}
	ft_2dstrfree(tmp);
	if (i != 3)
	ft_error(scene, "Missing color channel for floor!");
}

void	get_colors(t_scene *scene)
{
	int		i;
	int		j;
	char	**tmp;

	i = 0;
	tmp = NULL;
	while (scene->lines && scene->lines[i] && !(scene->f_color
			&& scene->c_color))
	{
		tmp = ft_split_wspaces(scene->lines[i]);
		j = 0;
		while (tmp && tmp[j])
		{
			if (ft_strnstr(tmp[j], "F", 1) && !scene->f_color)
				scene->f_color = ft_strdup(tmp[j + 1]);
			else if (ft_strnstr(tmp[j], "C", 1) && !scene->c_color)
				scene->c_color = ft_strdup(tmp[j + 1]);
			j++;
		}
		ft_2dstrfree(tmp);
		i++;
	}
	if (i >= scene->map_first_line)
		scene->map_first_line = i;
}
