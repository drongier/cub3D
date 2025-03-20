/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 17:03:47 by mekundur          #+#    #+#             */
/*   Updated: 2023/12/04 19:44:56 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	char	*str;

	str = s;
	while (n > 0)
	{
		str[--n] = c;
	}
	return (s);
}
/*
int	main(void)
{
	char	s[] = "MeRtCaN";

	printf("%s\n", s);
	ft_memset(s, 'C', 3);
	printf("%s\n", s);
}
*/
