/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 11:30:04 by mekundur          #+#    #+#             */
/*   Updated: 2023/12/11 18:57:41 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *lit, size_t len)
{
	size_t	i;
	size_t	j;

	if (*lit == '\0')
		return ((char *)big);
	i = 0;
	while (big[i] != '\0' && i < len)
	{
		j = 0;
		while (big[i + j] == lit[j] && i + j < len)
		{
			if (lit[j + 1] == '\0')
				return ((char *)&big[i]);
			else if (big[i + j + 1] == '\0')
				return (0);
			j++;
		}
		i++;
	}
	return (0);
}
/*
int	main(void)
{
	char	maxi[] = "abcdef";
	char	mini[] = "abcdefghijklmnop";

	printf("%s\n%s\n", maxi, mini);
	printf("\n%s", ft_strnstr(maxi, mini, 6));
	return (0);
}
*/
