/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 21:23:00 by mekundur          #+#    #+#             */
/*   Updated: 2023/12/06 21:46:08 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*str1;
	const char	*str2;
	size_t		i;

	str1 = dest;
	str2 = src;
	i = 0;
	while (i < n)
	{
		if (src < dest)
			str1[n - 1 - i] = str2[n - 1 - i];
		if (dest < src)
			str1[i] = str2[i];
		i++;
	}
	return (dest);
}
/*
int	main(void)
{
	ft_memmove("Mert", "can", 2);

	return (0);
}
*/
