/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:04:51 by mekundur          #+#    #+#             */
/*   Updated: 2024/01/29 15:18:35 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

static char	*ft_set_array(char *arr)
{
	char	*temp;
	char	*s;

	if (arr)
	{
		s = ft_gnl_strchr(arr, '\n');
		if (s == 0)
			return (free(arr), NULL);
		else
		{
			s++;
			if (*s == 0)
				return (free(arr), NULL);
			temp = (char *)ft_calloc((ft_strlen(s) + 1), sizeof(char));
			ft_gnl_strlcpy(temp, s, (ft_strlen(s) + 1));
			free(arr);
			arr = (char *)ft_calloc((ft_strlen(temp) + 1), sizeof(char));
			ft_gnl_strlcpy(arr, temp, (ft_strlen(temp) + 1));
			free(temp);
			return (arr);
		}
	}
	else
		return (NULL);
}

static char	*ft_put_line(char *arr)
{
	char	*s;
	char	*line;

	if (ft_gnl_strchr(arr, '\n') == 0)
	{
		line = (char *)ft_calloc((ft_strlen(arr) + 1), sizeof(char));
		if (!line)
			return (0);
		ft_gnl_strlcpy(line, arr, (ft_strlen(arr) + 1));
		return (line);
	}
	else
	{
		s = ft_gnl_strchr(arr, '\n');
		line = (char *)ft_calloc((s - arr + 2), sizeof(char));
		if (!line)
			return (0);
		ft_gnl_strlcpy(line, arr, (s - arr + 2));
		return (line);
	}
}

static char	*ft_read_line(int fd, char *arr)
{
	char	*buf;
	ssize_t	check;

	buf = (char *)ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buf)
		return (NULL);
	check = BUFFER_SIZE;
	while (!ft_gnl_strchr(arr, '\n') && check > 0)
	{
		check = read(fd, buf, BUFFER_SIZE);
		if (check == -1)
			return (free(buf), free(arr), NULL);
		if (check == 0)
			break ;
		arr = ft_gnl_strjoin(arr, buf, check);
	}
	return (free(buf), arr);
}

char	*get_next_line(int fd)
{
	static char	*arr[FOPEN_MAX];
	char		*line;

	if (fd == -1 || fd >= FOPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	arr[fd] = (ft_read_line(fd, arr[fd]));
	if (!arr[fd])
		return (free(arr[fd]), NULL);
	line = (ft_put_line(arr[fd]));
	arr[fd] = ft_set_array(arr[fd]);
	return (line);
}
/*
int	main(void)
{
	int	fd1, fd2, fd3, fd4, fd5;
	size_t	i;
	size_t	j;
	char	*line1;
	char	*line2;
	char	*buffer;

	i = 1;
	j = 1;
	line1 = "";
	fd1 = open("test_files/read_error.txt", O_RDONLY, 0);
//	printf("FD: %d\n", fd1);
	while (line1)
	{
		line1 = get_next_line(fd1);
		if (line1)
			printf("FD:%d %zu. %s", fd1, i, line1);
		fd2 = open("test_files/lines_around_10.txt", O_RDONLY, 0);		
//		printf("FD: %d\n", fd2);
		while (j <= 100) 
		{
        	        line2 = get_next_line(fd2);
                	if (line2)
                        	printf("FD:%d %zu. %s", fd2, j, line2);
	                free(line2);    
        	        j++;
		}
		fd3 = open("test_files/1char.txt", O_RDONLY, 0);	
		fd4 = open("test_files/42_with_nl.txt", O_RDONLY, 0);
		fd5 = open("test_files/test.txt", O_RDONLY, 0);
		buffer = (char *)ft_calloc(11, 1);
		read(fd5, buffer, 10);
//		printf("%s\n", buffer);	
		free(line1);	
		i++;
		close(fd2);
//		printf("%d\n", fd);
//		close(fd);
//		printf("%d\n", fd);
//		printf("%d\n", fd);
	}
	close(fd1);
//	printf("%d\n", _SC_OPEN_MAX);
	return (0);
}
*/
