/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_scene_data.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:30:38 by mekundur          #+#    #+#             */
/*   Updated: 2025/03/11 16:28:01 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	put_textures(t_scene *scene, char **tmp)
{
	int	j;

	j = 0;
	while (tmp && tmp[j])
	{
		if (ft_strnstr(tmp[j], "NO", 2) && ft_strnstr(tmp[j + 1], "./", 2))
			scene->no_texture = ft_strdup(tmp[j + 1]);
		else if (ft_strnstr(tmp[j], "SO", 2) && ft_strnstr(tmp[j + 1], "./", 2))
			scene->so_texture = ft_strdup(tmp[j + 1]);
		else if (ft_strnstr(tmp[j], "WE", 2) && ft_strnstr(tmp[j + 1], "./", 2))
			scene->we_texture = ft_strdup(tmp[j + 1]);
		else if (ft_strnstr(tmp[j], "EA", 2) && ft_strnstr(tmp[j + 1], "./", 2))
			scene->ea_texture = ft_strdup(tmp[j + 1]);
		j++;
	}
}

void	get_textures(t_scene *scene)
{
	int		i;
	char	**tmp;

	i = 0;
	tmp = NULL;
	while (scene->lines && scene->lines[i]
			&& !(scene->no_texture && scene->so_texture && scene->we_texture
			&& scene->ea_texture))
	{
		tmp = ft_split_wspaces(scene->lines[i]);
		put_textures(scene, tmp);
		ft_2dstrfree(tmp);
		i++;
	}
	scene->map_first_line = i;
}

int	ft_row_count(char *argv)
{
	int		row;
	int		fd;
	char	*tmp;

	row = 0;
	tmp = NULL;
	fd = open(argv, O_RDONLY);
	while (1)
	{
		tmp = get_next_line(fd);
		if (!tmp)
			break ;
		free(tmp);
		row++;
	}
	close(fd);
	return (row);
}

void	get_lines(char *argv, t_scene *scene)
{
	int		i;
	int		fd;

	scene->lines = (char **)calloc(scene->row + 1, sizeof(char *));
	if (!scene->lines)
		return ;
	scene->lines[scene->row] = NULL;
	fd = open(argv, O_RDONLY);
	i = 0;
	while (i < scene->row)
		scene->lines[i++] = get_next_line(fd);
	close (fd);
}

void	get_colors(t_scene *scene)
{
	int		i;
	int		j;
	char	**tmp;

	i = 0;
	tmp = NULL;
	while (scene->lines && scene->lines[i]
			&& !(scene->f_color && scene->c_color))
	{
		tmp = ft_split_wspaces(scene->lines[i]);
		j = 0;
		while (tmp && tmp[j])
		{
			if (ft_strnstr(tmp[j], "F", 1))
				scene->f_color = ft_strdup(tmp[j + 1]);
			else if (ft_strnstr(tmp[j], "C", 1))
				scene->c_color = ft_strdup(tmp[j + 1]);
			j++;
		}
		ft_2dstrfree(tmp);
		i++;
	}
	if (i >= scene->map_first_line)
		scene->map_first_line = i;
}

void	get_start_and_end_of_the_map(t_scene *scene)
{
	int	i;

	i = scene->map_first_line + 1;
	while (i < scene->row && ft_is_emptyline(scene->lines[i]))
		i++;
	scene->map_first_line = i;
	while (i < scene->row && !ft_is_emptyline(scene->lines[i]))
		i++;
	scene->map_last_line = i - 1;
	// printf("i: %d\n", i);
	// printf("%d\n", scene->map_last_line);
	while (i < scene->row && ft_is_emptyline(scene->lines[i]))
			i++;
	if (i != scene->row)
		ft_error(scene);
}

void	get_scene_data(char *argv, t_scene *scene)
{
	scene->row = ft_row_count(argv);
	get_lines(argv, scene);
	get_textures(scene);
	get_colors(scene);
	if (scene->no_texture && scene->so_texture && scene->we_texture   
		&& scene->ea_texture && scene->f_color && scene->c_color)
		get_start_and_end_of_the_map(scene);
	else
		ft_error(scene);		
	parse_map(scene);

}
