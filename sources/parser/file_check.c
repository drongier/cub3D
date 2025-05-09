/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:20:39 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/09 18:58:15 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	ft_textures_files_check(t_scene *scene)
{
    if (access(scene->no_texture, R_OK))
        ft_error(scene, "Error: NO texture file doesn't exist or have the read permission");
    if (access(scene->so_texture, R_OK))
        ft_error(scene, "Error: SO texture file doesn't exist or have the read permission");
    if (access(scene->we_texture, R_OK))
        ft_error(scene, "Error: WE texture file doesn't exist or have the read permission");
    if (access(scene->ea_texture, R_OK))
        ft_error(scene, "Error: EA texture file doesn't exist or have the read permission");       
}

void	ft_config_file_check(char *arg)
{
	int		fd;
    char    *tmp;
    char    *s;
    
	fd = open(arg, O_RDONLY);
	close (fd);
    if (fd == -1)
	 	ft_error(0, "Error: Config file doesn't exist or have the read permission!");
    tmp = ft_strtrim(arg, " \t");
    s = ft_strrchr(tmp, '.');
    if (ft_strncmp(s + 1, "cub", 3))
    {
        printf("Error: Configuration file format should be '.cub' !\n");
        free (tmp);
        exit (1);
    }
    free (tmp);  
}

