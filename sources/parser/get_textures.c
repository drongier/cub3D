/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_textures.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:25:47 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/12 13:17:42 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	put_no_texture(t_scene *scene, char **tmp)
{
	if (scene->no_texture || (*(tmp + 1) && ft_strlen(*(tmp + 1)) < 5))
	{
		ft_2dstrfree(tmp);
		ft_error(scene, "Multiple/missing NO texture path!");
	}
	else
	{
		scene->no_texture = ft_strdup(*(tmp + 1));
		ft_remove_trailing_new_line(scene->no_texture);
	}
	scene->texture_count++;
}

void	put_so_texture(t_scene *scene, char **tmp)
{
	if (scene->so_texture || (*(tmp + 1) && ft_strlen(*(tmp + 1)) < 5))
	{
		ft_2dstrfree(tmp);
		ft_error(scene, "Multiple/missing SO texture path!");
	}
	else
	{
		scene->so_texture = ft_strdup(*(tmp + 1));
		ft_remove_trailing_new_line(scene->so_texture);
	}
	scene->texture_count++;
}

void	put_ea_texture(t_scene *scene, char **tmp)
{
	if (scene->ea_texture || (*(tmp + 1) && ft_strlen(*(tmp + 1)) < 5))
	{
		ft_2dstrfree(tmp);
		ft_error(scene, "Multiple/missing EA texture path!");
	}
	else
	{
		scene->ea_texture = ft_strdup(*(tmp + 1));
		ft_remove_trailing_new_line(scene->ea_texture);
	}
	scene->texture_count++;
}

void	put_we_texture(t_scene *scene, char **tmp)
{
	if (scene->we_texture || (*(tmp + 1) && ft_strlen(*(tmp + 1)) < 5))
	{
		ft_2dstrfree(tmp);
		ft_error(scene, "Multiple/missing WE texture path!");
	}
	else
	{
		scene->we_texture = ft_strdup(*(tmp + 1));
		ft_remove_trailing_new_line(scene->we_texture);
	}
	scene->texture_count++;
}

void	get_textures(t_scene *scene)
{
	int		i;
	char	**tmp;

	i = 0;
	tmp = NULL;
	while (scene->lines && scene->lines[i])
	{
		tmp = ft_split_wspaces(scene->lines[i]);
		if (tmp && *tmp && *(tmp + 1) && \
			!ft_strncmp(*tmp, "NO", ft_strlen(*tmp)))
			put_no_texture(scene, tmp);
		else if (tmp && *tmp && !ft_strncmp(*tmp, "SO", ft_strlen(*tmp)))
			put_so_texture(scene, tmp);
		else if (tmp && *tmp && !ft_strncmp(*tmp, "WE", ft_strlen(*tmp)))
			put_we_texture(scene, tmp);
		else if (tmp && *tmp && !ft_strncmp(*tmp, "EA", ft_strlen(*tmp)))
			put_ea_texture(scene, tmp);
		ft_2dstrfree(tmp);
		if (scene->texture_count == 4 && !scene->map_first_line)
			scene->map_first_line = i;
		i++;
	}
}
