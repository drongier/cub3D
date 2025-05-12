/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:20:39 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/12 18:16:07 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	ft_xpm_check(t_scene *scene, char *texture)
{
	char	*tmp;
	char	*s;

	if (!texture)
		ft_error(scene, "Missing a texture path!");
	tmp = ft_strtrim(texture, " \t\n");
	s = ft_strrchr(tmp, '.');
	if (!ft_strchr(texture, '.') || ft_strncmp(s + 1, "xpm", 3))
	{
		free(tmp);
		ft_error(scene, "Texture files' format should be '.xpm' !");
	}
	free(tmp);
}

void	ft_textures_files_check(t_scene *scene)
{
	int	fd;

	if (!(scene->no_texture && scene->so_texture && scene->we_texture
			&& scene->ea_texture))
		ft_error(scene, "Missing a texture path!");
	ft_xpm_check(scene, scene->no_texture);
	fd = open(scene->no_texture, O_RDONLY);
	if (fd == -1)
		ft_error(scene, "NO texture file doesn't exist | Bad read rights!");
	close(fd);
	ft_xpm_check(scene, scene->so_texture);
	fd = open(scene->so_texture, O_RDONLY);
	if (fd == -1)
		ft_error(scene, "SO texture file doesn't exist | Bad read rights!");
	close(fd);
	ft_xpm_check(scene, scene->we_texture);
	fd = open(scene->we_texture, O_RDONLY);
	if (fd == -1)
		ft_error(scene, "WE texture file doesn't exist | Bad read rights!");
	close(fd);
	ft_xpm_check(scene, scene->ea_texture);
	fd = open(scene->ea_texture, O_RDONLY);
	if (fd == -1)
		ft_error(scene, "EA texture file doesn't exist | Bad read rights!");
	close(fd);
}

void	ft_config_file_check(char *arg)
{
	int		fd;
	char	*tmp;
	char	*s;

	fd = open(arg, O_RDONLY);
	close(fd);
	if (fd == -1)
	{
		printf("Error: Config file doesn't exist or have the read rights!\n");
		exit(1);
	}
	tmp = ft_strtrim(arg, " \t");
	s = ft_strrchr(tmp, '.');
	if (!s || ft_strncmp(s + 1, "cub", 3))
	{
		printf("Error: Bad file or bad file extension!\n");
		if (tmp)
			free(tmp);
		exit(1);
	}
	free(tmp);
}
