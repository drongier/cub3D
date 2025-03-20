/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 18:02:40 by mekundur          #+#    #+#             */
/*   Updated: 2024/02/19 03:42:43 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int	ptrflag_search2(unsigned long long num, const char *format, t_type *flags)
{
	int	n;
	int	len;

	n = 0;
	len = hexlen(num) + 2;
	while (flags->width-- > len)
		n += write(1, " ", 1);
	n += write(1, "0x", 2);
	return (n += ft_printhex(num, format));
}

int	ptrflag_search(unsigned long long num, const char *format, t_type *flags)
{
	int	n;
	int	len;

	n = 0;
	len = hexlen(num) + 2;
	if (flags->minus == 1)
	{
		n += write(1, "0x", 2);
		n += ft_printhex(num, format);
		while ((flags->width--) > len)
			n += write(1, " ", 1);
		return (n);
	}
	while (flags->minus && flags->width-- > len)
	{
		if (flags->zero == 1)
			n += write(1, "0", 1);
		else
			n += write(1, " ", 1);
	}
	if (!flags->minus)
		return (n += ptrflag_search2(num, format, flags));
	else
		return (n += ft_printhex(num, format));
}

int	ptr_null_print(t_type *flags)
{
	int	n;

	n = 0;
	if (!flags)
		n += write(1, "(nil)", 5);
	else if (flags && flags->minus)
	{
		n += write(1, "(nil)", 5);
		while (flags->width-- > 5)
			n += write(1, " ", 1);
		return (n);
	}
	else if (flags && !flags->minus)
	{
		while (flags->width-- > 5)
			n += write(1, " ", 1);
		return (n += write(1, "(nil)", 5));
	}
	return (n);
}

int	ft_putptr(unsigned long long num, const char *format, t_type *flags)
{
	int	n;

	n = 0;
	if (num == 0)
		return (ptr_null_print(flags));
	if (flags)
		n += ptrflag_search(num, format, flags);
	else
	{
		n += write (1, "0x", 2);
		n += ft_printhex(num, format);
	}
	return (n);
}
