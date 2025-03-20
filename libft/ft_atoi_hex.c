/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_hex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 17:27:41 by mekundur          #+#    #+#             */
/*   Updated: 2023/12/09 17:40:42 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

static int	ft_isspace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

static void	hex_conversion(char *nptr, int *hex)
{
	*hex *= 16;
	if (*nptr >= 'A' && *nptr <= 'F')
		*hex += *nptr - 65 + 10;
	else if (*nptr >= '0' && *nptr <= '9')
		*hex += *nptr - 48;
}

int	ft_atoi_hex(const char *nptr)
{
	int	*hex;
	int	num;

	num = 0;
	hex = &num;
	if (!nptr || !*nptr)
		return (0);
	while (ft_isspace(*nptr) == 1)
		nptr++;
	if (*nptr == '0' && *(nptr + 1) == 'x')
		nptr += 2;
	else
		return (0);
	while (*nptr)
	{
		hex_conversion((char *)nptr, hex);
		nptr++;
	}
	return (num);
}
/*
int	main(void)
{
	printf("%u\n", ft_atoi_hex("0xFFFFFFF"));
	printf("%u\n", ft_atoi_hex("0x0"));
	printf("%u\n", ft_atoi_hex("0x10A"));
	printf("%u\n", ft_atoi_hex("0xGT"));
	printf("%u\n", ft_atoi_hex("0x"));
	printf("%u\n", ft_atoi_hex("0x02"));
	printf("%u\n", ft_atoi_hex("0x64"));
	return (0);
}*/
