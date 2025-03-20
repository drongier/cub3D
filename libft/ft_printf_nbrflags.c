/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nbrflags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 18:02:06 by mekundur          #+#    #+#             */
/*   Updated: 2024/02/19 06:23:10 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int	nbrflag_minus(long long num, long long *ptr, t_type *flags, int n)
{
	if (num < 0)
	{
		num *= -1;
		n += write(1, "-", 1);
	}
	if (!flags->prcsn && flags->space && *ptr >= 0)
		n += write(1, " ", 1);
	if (!flags->prcsn)
		n += printnbr(num, ptr, flags);
	else
	{
		while (flags->prcsn-- > ft_diglen(num))
			n += write(1, "0", 1);
		n += printnbr(num, ptr, flags);
		while (flags->width > n && flags->width > 0)
			n += write(1, " ", 1);
		return (n);
	}
	while (flags->width > n && flags->width > 0)
		n += write(1, " ", 1);
	return (n);
}

int	nbrflag_prcsn(long long num, long long *ptr, t_type *flags, int n)
{
	int	len;
	int	neg;

	neg = 0;
	if (num < 0)
		neg++;
	len = ft_diglen(num);
	while ((flags->prcsn >= len)
		&& (flags->width-- > flags->prcsn + neg && flags->width > 0))
		n += write(1, " ", 1);
	while ((flags->prcsn < len)
		&& (flags->width-- > len && flags->width > 0))
		n += write(1, " ", 1);
	if (num < 0)
	{
		n += write(1, "-", 1);
		len--;
		*ptr *= -1;
	}
	while (flags->prcsn-- > len && flags->prcsn > 0)
		n += write(1, "0", 1);
	return (n);
}

int	nbrflag_space_width(long long num, long long *ptr, t_type *flags, int n)
{
	int	len;

	len = ft_diglen(num);
	if (flags->space && num >= 0)
		n += write(1, " ", 1);
	while (flags->width > len + n)
		n += write(1, " ", 1);
	return (n += printnbr(num, ptr, flags));
}

int	nbrflag_plus_width(long long num, long long *ptr, t_type *flags, int n)
{
	int	len;

	len = ft_diglen(num);
	if (flags->plus && num >= 0)
		len++;
	while (flags->width > len + n)
		n += write(1, " ", 1);
	if (flags->plus && num >= 0)
		n += write(1, "+", 1);
	return (n += printnbr(num, ptr, flags));
}

int	nbrflag_search(long long num, long long *ptr, t_type *flags, int n)
{
	int	len;

	len = ft_diglen(num);
	while ((flags->dot && !flags->prcsn && num == 0 && flags->width-- > 0))
		n += write(1, " ", 1);
	if (flags->prcsn && !flags->minus)
		return (n += nbrflag_prcsn(num, ptr, flags, n));
	else
	{
		while (flags->width-- > len && flags->width > 0)
		{
			if (*ptr < 0 && flags->zero && !flags->dot)
			{
				n += write(1, "-", 1);
				*ptr *= -1;
			}
			if (flags->zero && !flags->dot)
				n += write(1, "0", 1);
			else
				n += write(1, " ", 1);
		}
	}
	return (n);
}
