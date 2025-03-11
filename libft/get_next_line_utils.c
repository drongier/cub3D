/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:07:45 by mekundur          #+#    #+#             */
/*   Updated: 2024/01/24 19:23:31 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

char	*ft_gnl_strlcpy(char *dst, char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size != 0)
	{
		while (src[i] != '\0' && i < size - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (dst);
}

char	*ft_gnl_strjoin(char *arr, char *buf, ssize_t check)
{
	size_t	len1;
	size_t	i;
	char	*str;

	i = 0;
	if (!arr)
		arr = (char *)ft_calloc(1, sizeof(char));
	if (!arr)
		return (NULL);
	len1 = ft_strlen(arr);
	str = (char *)ft_calloc((len1 + check + 1), sizeof(char));
	if (!str)
		return (free(arr), NULL);
	while (i < len1)
	{
		str[i] = arr[i];
		i++;
	}
	while (buf[i - len1] && check > 0)
	{
		str[i] = buf[i - len1];
		i++;
		check--;
	}
	return (free(arr), str);
}

char	*ft_gnl_strchr(char *s, int c)
{
	if (!s)
		return (0);
	while (*s != (char)c && *s != '\0')
		s++;
	if (*s == (char)c)
		return (s);
	else
		return (0);
}
