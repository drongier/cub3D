/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_textures.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:25:47 by mekundur          #+#    #+#             */
/*   Updated: 2025/03/21 12:29:38 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	put_textures_contd(t_scene *scene, char **tmp)
{
	if (!tmp)
		return ;
	else if (ft_strnstr(*tmp, "WE", 2))
	{
		if (*(tmp + 1))
		{
			scene->we_texture = ft_strdup(*(tmp + 1));
			ft_remove_trailing_new_line(scene->we_texture);
		}
		else
			ft_error(scene);
	}
	else if (ft_strnstr(*tmp, "EA", 2))
	{
		if (*(tmp + 1))
		{
			scene->ea_texture = ft_strdup(*(tmp + 1));
			ft_remove_trailing_new_line(scene->ea_texture);
		}
		else
			ft_error(scene);
	}
}

void	put_textures(t_scene *scene, char **tmp)
{
	while (tmp && *tmp)
	{
		if (ft_strnstr(*tmp, "NO", 2))
		{
			if (*(tmp + 1))
			{
				scene->no_texture = ft_strdup(*(tmp + 1));
				ft_remove_trailing_new_line(scene->no_texture);
			}
			else
				ft_error(scene);
		}
		else if (ft_strnstr(*tmp, "SO", 2))
		{
			if (*(tmp + 1))
			{
				scene->so_texture = ft_strdup(*(tmp + 1));
				ft_remove_trailing_new_line(scene->so_texture);
			}
			else
				ft_error(scene);
		}
		put_textures_contd(scene, tmp);
		tmp++;
	}
}

void	get_textures(t_scene *scene)
{
	int		i;
	char	**tmp;

	i = 0;
	tmp = NULL;
	while (scene->lines && scene->lines[i] && !(scene->no_texture
			&& scene->so_texture && scene->we_texture && scene->ea_texture))
	{
		tmp = ft_split_wspaces(scene->lines[i]);
		put_textures(scene, tmp);
		ft_2dstrfree(tmp);
		i++;
	}
	scene->map_first_line = i;
}
