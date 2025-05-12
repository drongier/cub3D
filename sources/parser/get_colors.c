/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_colors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:29:08 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/12 13:32:50 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	encode_ceiling_color(t_scene *scene, t_map *map)
{
	char	**tmp;
	int		i;
	int		num;

	tmp = ft_split(scene->c_color, ',');
	if (!tmp)
		ft_error(scene, "C Color issue!");
	i = 0;
	while (tmp[i])
	{
		num = ft_atoi(tmp[i]);
		if (!(num >= 0 && num <= 255)
			|| (i < 3 && (!tmp[i][0] || tmp[i][0] == '\n')))
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

	tmp = ft_split(scene->f_color, ',');
	if (!tmp)
		ft_error(scene, "F Color issue!");
	i = 0;
	while (tmp[i])
	{
		num = ft_atoi(tmp[i]);
		if (!(num >= 0 && num <= 255)
			|| (i < 3 && (!tmp[i][0] || tmp[i][0] == '\n')))
		{
			ft_2dstrfree(tmp);
			ft_error(scene, "Wrong color format for ceiling!");
		}
		map->floor = map->floor << 8;
		map->floor += num;
		i++;
	}
	ft_2dstrfree(tmp);
	if (i != 3)
		ft_error(scene, "Missing/redundant color channel for ceiling!");
}

void	get_color_inner_loop(t_scene *scene, char *tmp)
{
	if (((tmp && *tmp == 'F' && *(tmp + 1) == ' ') && scene->f_color) || ((tmp
				&& *tmp == 'C' && *(tmp + 1) == ' ') && scene->c_color))
	{
		free(tmp);
		ft_error(scene, "Multiple color entry!");
	}
	else if (tmp && *tmp == 'F' && *(tmp + 1) == ' ')
	{
		scene->f_color = ft_strdup(tmp + 1);
		scene->color_count++;
	}
	else if (tmp && *tmp == 'C' && *(tmp + 1) == ' ')
	{
		scene->c_color = ft_strdup(tmp + 1);
		scene->color_count++;
	}
}

void	get_colors(t_scene *scene)
{
	int		i;
	char	*tmp;
	bool	flag;

	i = 0;
	flag = 1;
	tmp = NULL;
	while (scene->lines && scene->lines[i])
	{
		tmp = ft_strtrim(scene->lines[i], " ");
		get_color_inner_loop(scene, tmp);
		free(tmp);
		if (scene->color_count == 2 && i >= scene->map_first_line && flag)
		{
			scene->map_first_line = i;
			flag = 0;
		}
		i++;
	}
}
