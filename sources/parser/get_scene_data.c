/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_scene_data.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:30:38 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/09 19:11:09 by mekundur         ###   ########.fr       */
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
	 	ft_error(0, "Open error!");
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
	int j;
	int	fd;
	int	len;

	scene->lines = (char **)calloc(scene->row + 1, sizeof(char *));
	if (!scene->lines)
		ft_cleanup(scene);
	scene->lines[scene->row] = NULL;
	fd = open(argv, O_RDONLY);
	i = -1;
	while (++i < scene->row)
	{	
		scene->lines[i] = get_next_line(fd);
		len = ft_strlen(scene->lines[i]);
		// printf("len :%d %s", len, scene->lines[i]);
		j = 0;
		while (scene->lines[i][j] != '\0')
			j++;
		if (scene->lines[i][j - 1] != '\n')
			scene->lines[i][j - 1] = '\n';
		// printf("len :%d %s", len, scene->lines[i]);
	}
	close(fd);
}

void	get_start_and_end_of_the_map(t_scene *scene)
{
	int	i;
	
	// printf("FIRST: %d\n", scene->map_first_line);

	i = scene->map_first_line;
	while (i < scene->row && ft_is_emptyline(scene->lines[i]))
		i++;
	// printf("FIRST: %d\n", scene->map_first_line);
	scene->map_first_line = i;
	// printf("FIRST: %d\n", scene->map_first_line);
	printf("i: %d\n", i);
	while (i < scene->row && !ft_is_emptyline(scene->lines[i]))
		i++;
	printf("i: %d\n", i);	
	scene->map_last_line = i ;
	scene->map->row = scene->map_last_line - scene->map_first_line;

	printf("i: %d\n", i);
	printf("FIRST: %d\n", scene->map_first_line);
	printf("LAST: %d\n", scene->map_last_line);

	while (i < scene->row && ft_is_emptyline(scene->lines[i]))
		i++;
	printf("map_row %d\n", scene->map->row);

	printf("i %d ROW %d\n", i, scene->row);

	if (i != scene->row)
		ft_error(scene, "There's mysterious stuff after map!");
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
		ft_error(scene, "Missing color/texture!");
	parse_map(scene);
}
