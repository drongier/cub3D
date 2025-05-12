/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_scene_data.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:30:38 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/12 13:59:39 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	ft_row_count(t_scene *scene, char *argv)
{
	int		row;
	int		fd;
	char	*tmp;

	row = 0;
	tmp = NULL;
	fd = open(argv, O_RDONLY);
	if (fd == -1)
		ft_error(scene, "Open error!");
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
	int	i;
	int	j;
	int	fd;

	scene->lines = (char **)ft_calloc(scene->row + 1, sizeof(char *));
	if (!scene->lines)
		ft_error(scene, "Allocation error!");
	scene->lines[scene->row] = NULL;
	fd = open(argv, O_RDONLY);
	i = -1;
	while (++i < scene->row)
	{
		scene->lines[i] = get_next_line(fd);
		j = 0;
		while (scene->lines[i][j] != '\0')
			j++;
	}
	close(fd);
}

void	get_start_and_end_of_the_map(t_scene *scene)
{
	int	i;

	scene->map_first_line++;
	i = scene->map_first_line;
	while (i < scene->row && ft_is_emptyline(scene->lines[i]))
		i++;
	scene->map_first_line = i;
	while (i < scene->row && !ft_is_emptyline(scene->lines[i]))
		i++;
	scene->map_last_line = i ;
	scene->map->row = scene->map_last_line - scene->map_first_line;
	while (i < scene->row && ft_is_emptyline(scene->lines[i]))
		i++;
	if (i != scene->row)
		ft_error(scene, "There's mysterious stuff before/after the map!");
	if (scene->map_last_line <= scene->map_first_line)
		ft_error(scene, "Map is no at the end of the file!");
}

void	get_scene_data(char *argv, t_scene *scene)
{
	scene->row = ft_row_count(scene, argv);
	get_lines(argv, scene);
	get_textures(scene);
	ft_textures_files_check(scene);
	get_colors(scene);
	get_start_and_end_of_the_map(scene);
	parse_map(scene);
}
