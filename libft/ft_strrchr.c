/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 16:29:42 by mekundur          #+#    #+#             */
/*   Updated: 2023/12/09 19:24:37 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*str;

	str = s + ft_strlen(s);
	if (c == '\0')
		return ((char *)str);
	while (str != s || *s == (char)c)
	{
		if (*str == (char)c)
			return ((char *)str);
		str--;
	}
	return (0);
}
/*
int     main(void)
{
	const char *s;
	printf("%p\n", s);
	ft_strrchr("teste", '\0');
	printf("%s\n%p\n", s, s);

	return (0);
}
*/
