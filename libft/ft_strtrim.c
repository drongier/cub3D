/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 11:33:02 by mekundur          #+#    #+#             */
/*   Updated: 2023/12/11 11:33:21 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_same_char(const char *s1, const char *s2)
{
	size_t	j;

	j = 0;
	while (s2[j] != '\0')
	{
		if (s2[j] == *s1)
		{
			return (1);
		}
		else
			j++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	j;
	size_t	len;
	char	*str;	

	j = 0;
	len = ft_strlen(s1);
	while (ft_same_char(s1, set) == 1 && s1)
	{
		s1++;
		j++;
	}
	while (ft_same_char((s1 + len - j - 1), set) == 1 && len > j)
		j++;
	str = (char *)malloc((len - j + 1) * sizeof(char));
	if (!str)
		return (0);
	ft_memcpy(str, s1, len - j);
	*(str + len - j) = '\0';
	return (str);
}
