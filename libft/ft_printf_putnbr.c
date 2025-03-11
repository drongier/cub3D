/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 18:00:11 by mekundur          #+#    #+#             */
/*   Updated: 2024/02/19 06:28:14 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int	printnbr(long long num, long long *ptr, t_type *flags)
{
	int	n;

	n = ft_diglen(num);
	if (num == -9223372036854775807)
		n += write(1, "-9223372036854775807", 20);
	else if (num < 0)
	{
		num *= -1;
		*ptr *= -1;
		write(1, "-", 1);
		printnbr(num, ptr, flags);
	}
	else if (num >= 10)
	{
		printnbr(num / 10, ptr, flags);
		printnbr(num % 10, ptr, flags);
	}
	else if (num >= 0 && num <= 9)
	{
		num = num + '0';
		write(1, &num, 1);
	}
	return (n);
}

int	nbrflag_space_zero(long long num, long long *ptr, t_type *flags, int n)
{
	int	len;

	len = ft_diglen(num);
	ptr = ptr + 0;
	if (flags->space && num >= 0)
		n += write(1, " ", 1);
	if (num < 0)
	{
		n += write (1, "-", 1);
		num *= -1;
		len--;
	}
	if (flags->plus && *ptr >= 0)
		n += write(1, "+", 1);
	while (flags->zero && flags->width > len + n)
		n += write(1, "0", 1);
	return (n += printnbr(num, ptr, flags));
}

int	nbrflag_plus(long long num, long long *ptr, t_type *flags, int n)
{
	if (flags->minus && num < 0)
	{
		num *= -1;
		n += write(1, "-", 1);
		n += printnbr(num, ptr, flags);
		while (flags->width > n)
			n += write(1, " ", 1);
		return (n);
	}
	ptr = ptr + 0;
	if (flags->minus && num >= 0)
	{
		n += write(1, "+", 1) + printnbr(num, ptr, flags);
		while (flags->width > n)
			n += write(1, " ", 1);
		return (n);
	}
	if (num < 0)
	{
		num *= -1;
		n += write(1, "-", 1);
	}
	else
		n += write(1, "+", 1);
	return (n += printnbr(num, ptr, flags));
}

int	ft_putnbr(long long num, t_type *flags)
{
	long long	*ptr;
	int			n;

	ptr = &num;
	n = 0;
	if (flags && flags->dot && !flags->prcsn && num == 0)
		return (n += nbrflag_search(num, ptr, flags, n));
	else if (flags && flags->minus && !flags->plus)
		return (n += nbrflag_minus(num, ptr, flags, n));
	else if (flags && flags->space && flags->zero)
		return (n += nbrflag_space_zero(num, ptr, flags, n));
	else if (flags && flags->plus && flags->zero)
		return (n += nbrflag_space_zero(num, ptr, flags, n));
	else if (flags && flags->plus && flags->width && !flags->minus)
		return (n += nbrflag_plus_width(num, ptr, flags, n));
	else if (flags && flags->space && flags->width)
		return (n += nbrflag_space_width(num, ptr, flags, n));
	else if (flags && flags->plus)
		return (n += nbrflag_plus(num, ptr, flags, n));
	else if (flags && flags->space && num >= 0)
		n += write(1, " ", 1);
	else if (flags)
		n += nbrflag_search(num, ptr, flags, n);
	return (n += printnbr(num, ptr, flags));
}
