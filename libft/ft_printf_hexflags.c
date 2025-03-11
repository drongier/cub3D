/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexflags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 01:02:15 by mekundur          #+#    #+#             */
/*   Updated: 2024/02/19 06:40:00 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int	hexfl_min(unsigned long long num, const char *format, t_type *flags, int n)
{
	int	len;

	len = hexlen(num);
	if (flags->hash)
		n += hexfl_hash(format);
	if (!flags->prcsn)
		n += ft_printhex(num, format);
	else
	{
		while (flags->prcsn-- > len)
			n += write (1, "0", 1);
		n += ft_printhex(num, format);
	}
	while (flags->width > n && flags->width > 0)
		n += write(1, " ", 1);
	return (n);
}

int	hexfl_pre(unsigned long long num, const char *format, t_type *flags, int n)
{
	int	len;

	len = hexlen(num);
	while (num == 0 && (flags->prcsn-- > len))
		n += write(1, "0", 1);
	while ((flags->prcsn >= len)
		&& (flags->width-- > flags->prcsn + flags->hash))
		n += write(1, " ", 1);
	while ((flags->prcsn < len)
		&& (flags->width-- > len + flags->hash))
		n += write(1, " ", 1);
	if (flags->hash)
		n += hexfl_hash(format);
	while (flags->prcsn-- > len && flags->prcsn > 0)
		n += write(1, "0", 1);
	return (n);
	format = format + 0;
}

int	hexfl_hash(const char *format)
{
	if (*format == 'x')
		write(1, "0x", 2);
	else if (*format == 'X')
		write(1, "0X", 2);
	return (2);
}

int	hexfl_sch(unsigned long long num, const char *format, t_type *flags, int n)
{
	n = 0;
	while (flags->dot && !flags->prcsn && num == 0 && flags->width-- > 0)
	{
		n += write(1, " ", 1);
		return (n);
	}
	if (flags->minus)
		return (n += hexfl_min(num, format, flags, n));
	if (flags->zero && flags->hash)
		n += hexfl_hash(format);
	while (!flags->prcsn && flags->zero
		&& flags->width-- > hexlen(num) + flags->hash)
		n += write(1, "0", 1);
	while (!flags->prcsn && !flags->zero
		&& flags->width-- > hexlen(num) + flags->hash)
		n += write(1, " ", 1);
	if (flags->hash && !flags->zero && !flags->prcsn)
		n += hexfl_hash(format);
	if (flags->prcsn)
	{
		n += hexfl_pre(num, format, flags, n);
		return (n += ft_printhex(num, format));
	}
	return (n += ft_printhex(num, format));
}
