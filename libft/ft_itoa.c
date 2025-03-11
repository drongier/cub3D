/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 11:34:30 by mekundur          #+#    #+#             */
/*   Updated: 2023/12/11 11:34:32 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_diglen(long int n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		len++;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char		*rev;
	int			len;
	long int	num;

	len = ft_diglen(n);
	rev = (char *)malloc(len * sizeof(char) + 1);
	if (!rev)
		return (0);
	if (n == 0)
		rev[0] = 0 + 48;
	num = n;
	if (n < 0)
	{
		rev[0] = '-';
		num *= -1;
	}
	rev[len--] = '\0';
	while (len >= 0 && num)
	{
		rev[len--] = num % 10 + 48;
		num = num / 10;
	}
	return (rev);
}
/*
int	main(void)
{
	int		num;
	char	*s;
	int		digit;
	int		tmp;

	num = -2147483648;
	tmp = num;
	digit = 0;
	printf("Int: %d\n", num);
	while (num > 0)
	{
		num = num / 10;
		digit++;
	}
	s = (char *)malloc(digit * sizeof(char) + 1);
	s = ft_itoa(tmp);
	printf("Str: %s", s);	
	return (0);
}
*/
