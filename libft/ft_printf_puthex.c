/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 18:00:52 by mekundur          #+#    #+#             */
/*   Updated: 2024/02/19 02:54:11 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int	hexlen(unsigned long long num)
{
	int	len;

	len = 0;
	if (num == 0)
		return (1);
	while (num >= 1)
	{
		num = num / 16;
		len++;
	}
	return (len);
}

int	ft_printhex(unsigned long long num, const char *format)
{
	int	n;

	n = hexlen(num);
	if (num >= 16)
	{
		ft_printhex(num / 16, format);
		ft_printhex(num % 16, format);
	}
	else if (num <= 9)
	{
		num += '0';
		write(1, &num, 1);
	}
	else if (num >= 10 && num <= 15)
	{
		if (*format == 'x' || *format == 'p')
			num = num - 10 + 'a';
		else if (*format == 'X')
			num = num - 10 + 'A';
		write(1, &num, 1);
	}
	return (n);
}

int	hex_null_print2(int n, t_type *flags)
{
	while (flags && !flags->prcsn && !flags->zero && --flags->width > 0)
		n += write(1, " ", 1);
	while (flags && !flags->prcsn && flags->zero && --flags->width > 0)
		n += write(1, "0", 1);
	while (flags && flags->width-- > flags->prcsn)
		n += write(1, " ", 1);
	while (flags && --flags->prcsn > 0)
		n += write(1, "0", 1);
	return (n += write(1, "0", 1));
}

int	hex_null_print(int n, t_type *flags)
{
	if (flags && flags->dot && !flags->prcsn)
	{
		while (flags->width-- > 0)
			n += write(1, " ", 1);
		return (n);
	}
	else if (flags && !flags->width && (flags->zero || flags->space))
		return (write(1, "0", 1));
	else if (flags && flags->minus)
	{
		n += write(1, "0", 1);
		while (flags->prcsn > n)
			n += write(1, "0", 1);
		while (flags->width > n)
			n += write(1, " ", 1);
		return (n);
	}
	else
		return (n += hex_null_print2(n, flags));
}

int	ft_puthex(unsigned long long num, const char *format, t_type *flags)
{
	int	n;

	n = 0;
	if (num == 0)
		return (hex_null_print(n, flags));
	if (flags)
	{
		if (flags && !flags->width && num == 0 && !flags->prcsn)
			return (write(1, "0", 1));
		if (flags->dot && !flags->prcsn && num == 0)
			return (n += hexfl_sch(num, format, flags, n));
		n += hexfl_sch(num, format, flags, n);
	}
	else
		n += ft_printhex(num, format);
	return (n);
}
