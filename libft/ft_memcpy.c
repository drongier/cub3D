/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 20:47:34 by mekundur          #+#    #+#             */
/*   Updated: 2023/12/04 21:19:24 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*str1;
	const char	*str2;
	size_t		i;

	str1 = dest;
	str2 = src;
	i = 0;
	if (dest == 0 && src == 0)
		return (dest);
	while (i < n)
	{
		str1[i] = str2[i];
		i++;
	}
	return (dest);
}
/*
int	main(void)
{
	char str1[] = "            ";
	char *str2 = "";

	ft_memcpy(str1, str2, 17);
//	printf("%s\n", str1);
// 	ft_memcpy("            ", 0, 17);
        printf("%s\n", str1);


	return (0);
}
*/
