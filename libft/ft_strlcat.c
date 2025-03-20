/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 12:41:43 by mekundur          #+#    #+#             */
/*   Updated: 2023/12/07 15:05:33 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dlen;

	i = 0;
	dlen = ft_strlen(dst);
	if (size > dlen)
	{
		while (src[i] != '\0' && ((dlen + i) < (size - 1)))
		{
			dst[dlen + i] = src[i];
			i++;
		}
		dst[dlen + i] = '\0';
		return (ft_strlen(src) + dlen);
	}
	else
		return (ft_strlen(src) + size);
}
/*
int	main(void)
{
	char dst[] = "there is no stars in the sky";
	char *src = "the cake is a lie !\0I'm hidden lol\r\n";

	printf("%s\n%s\n", dst, src);
	ft_strlcat(dst, src, 555);
	printf("%s\n", dst);

	return (0);
}
*/
