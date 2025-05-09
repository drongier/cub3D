/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_textures.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:25:47 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/09 15:19:19 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// void	put_textures_contd(t_scene *scene, char **tmp)
// {
// 	if (!tmp)
// 		return ;
// 	else if (ft_strnstr(*tmp, "WE", 2) && !scene->we_texture)
// 	{
// 		if (*(tmp + 1))
// 		{
// 			scene->we_texture = ft_strdup(*(tmp + 1));
// 			ft_remove_trailing_new_line(scene->we_texture);
// 		}
// 		else
// 			ft_error(scene, "Texture path is missing!");
// 	}
// 	else if (ft_strnstr(*tmp, "EA", 2) && !scene->ea_texture)
// 	{
// 		if (*(tmp + 1))
// 		{
// 			scene->ea_texture = ft_strdup(*(tmp + 1));
// 			ft_remove_trailing_new_line(scene->ea_texture);
// 		}
// 		else
// 			ft_error(scene, "Texture path is missing!");
// 	}
// }

void	put_no_texture(t_scene *scene, char **tmp)
{
	if (scene->no_texture || !*(tmp + 1))
	{
		ft_2dstrfree(tmp);
		ft_error(scene, "Multiple/missing WE texture path!");
	}
	else
	{
		scene->no_texture = ft_strdup(*(tmp + 1));
		ft_remove_trailing_new_line(scene->no_texture);
		// printf("%s\n", scene->no_texture);
	}
}

void	put_so_texture(t_scene *scene, char **tmp)
{
	if (scene->so_texture || !*(tmp + 1))
	{
		ft_2dstrfree(tmp);
		ft_error(scene, "Multiple/missing SO texture path!");
	}
	else
	{
		scene->so_texture = ft_strdup(*(tmp + 1));
		ft_remove_trailing_new_line(scene->so_texture);
		// printf("%s\n", scene->so_texture);
	}
}

void	put_ea_texture(t_scene *scene, char **tmp)
{		
	if (scene->ea_texture || !*(tmp + 1))
	{
		ft_2dstrfree(tmp);
		ft_error(scene, "Multiple/missing EA texture path!");
	}
	else
	{
		scene->ea_texture = ft_strdup(*(tmp + 1));
		ft_remove_trailing_new_line(scene->ea_texture);
		// printf("%s\n", scene->ea_texture);
	}
}

void	put_we_texture(t_scene *scene, char **tmp)
{
	if (scene->we_texture || !*(tmp + 1))
	{
		ft_2dstrfree(tmp);
		ft_error(scene, "Multiple/missing WE texture path!");
	}
	else
	{
		scene->we_texture = ft_strdup(*(tmp + 1));
		ft_remove_trailing_new_line(scene->we_texture);
		// printf("%s\n", scene->we_texture);
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
		// printf("%s", *tmp);
		if (tmp && *tmp && !ft_strncmp(*tmp, "NO", ft_strlen(*tmp)))
			put_no_texture(scene, tmp);
		else if (tmp && *tmp && !ft_strncmp(*tmp, "SO", ft_strlen(*tmp)))
			put_so_texture(scene, tmp);
		else if (tmp && *tmp && !ft_strncmp(*tmp, "WE", ft_strlen(*tmp)))
			put_we_texture(scene, tmp);
		else if (tmp && *tmp && !ft_strncmp(*tmp, "EA", ft_strlen(*tmp)))
			put_ea_texture(scene, tmp);
		ft_2dstrfree(tmp);
		i++;
	}
	// printf("%s\n", scene->no_texture);
	// printf("%s\n", scene->so_texture);
	// printf("%s\n", scene->we_texture);
	// printf("%s\n", scene->ea_texture);

	scene->map_first_line = i;
}
