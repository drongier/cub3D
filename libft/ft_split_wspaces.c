/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_wspaces.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:04:10 by mekundur          #+#    #+#             */
/*   Updated: 2025/03/04 16:51:34 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*free_ft_split_array(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	return (NULL);
}

static size_t	ft_string_count(char const *s)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		if (ft_is_whitespace(*s))
			s++;
		else
		{
			count++;
			while (*s && !ft_is_whitespace(*s))
				s++;
		}
	}
	return (count);
}

static char	**ft_put_string(char const *s1, char **arr, size_t wcount)
{
	size_t	i;
	char	*s2;
	char	*str;

	s2 = (char *)s1;
	i = 0;
	while (*s1 && i < wcount)
	{
		while (ft_is_whitespace(*s2))
		{
			s1++;
			s2++;
		}
		while (!ft_is_whitespace(*s2) && *s2 != '\0')
			s2++;
		str = (char *)malloc((s2 - s1 + 1) * sizeof(char));
		if (!str)
			return (free_ft_split_array(arr), NULL);
		ft_strlcpy(str, s1, (s2 - s1 + 1));
		arr[i++] = str;
		s1 = s2;
	}
	arr[i] = 0;
	return (arr);
}

char	**ft_split_wspaces(char const *s)
{
	char	**arr;
	size_t	wcount;

	if (!s)
		return (0);
	wcount = ft_string_count(s);
	arr = (char **)malloc((wcount + 1) * sizeof(char *));
	if (!arr)
		return (0);
	ft_put_string(s, arr, wcount);
	return (arr);
}
