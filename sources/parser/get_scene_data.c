/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_scene_data.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:30:38 by mekundur          #+#    #+#             */
/*   Updated: 2025/03/21 12:29:14 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	ft_row_count(char *argv)
{
	int		row;
	int		fd;
	char	*tmp;

	row = 0;
	tmp = NULL;
	fd = open(argv, O_RDONLY);
	if (fd == -1)
		ft_error(0);
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
	int	fd;

	scene->lines = (char **)calloc(scene->row + 1, sizeof(char *));
	if (!scene->lines)
		return ;
	scene->lines[scene->row] = NULL;
	fd = open(argv, O_RDONLY);
	i = 0;
	while (i < scene->row)
		scene->lines[i++] = get_next_line(fd);
	close(fd);
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
