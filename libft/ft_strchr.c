/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 17:07:04 by mekundur          #+#    #+#             */
/*   Updated: 2023/12/09 19:23:19 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s != (char)c && *s != '\0')
		s++;
	if (*s == (char)c)
		return ((char *)s);
	else
		return (0);
}
/*
int	main(void)
{
	const char *s = "teste";

	printf("%p\n", s);
	s = ft_strchr(s, 'e');
	printf("%p\n", s);
	printf("%s\n", s);

	return (0);
}
*/
