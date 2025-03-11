/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 14:48:15 by mekundur          #+#    #+#             */
/*   Updated: 2024/09/04 16:52:43 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 50
# endif

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>
# include <fcntl.h>
# include <limits.h>

char	*get_next_line(int fd);
char	*ft_gnl_strjoin(char *arr, char *buf, ssize_t check);
char	*ft_gnl_strchr(char *s, int c);
char	*ft_gnl_strlcpy(char *dst, char *src, size_t size);

#endif
