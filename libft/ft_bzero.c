/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 20:37:27 by mekundur          #+#    #+#             */
/*   Updated: 2023/12/04 20:45:02 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*str;

	str = s;
	while (n > 0)
		str[--n] = 0;
}
/*
int	main(void)
{
	char	s[] = "MERTCAN";

	printf("%s\n", s);
	ft_bzero(s, 3);
	printf("%s\n", s+2);

	return (0);
}
*/
