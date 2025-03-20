/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strutils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 21:20:22 by mekundur          #+#    #+#             */
/*   Updated: 2024/02/18 22:47:14 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int	strflag_width_prcsn(char *s, t_type *flags, int len)
{
	int	n;

	n = 0;
	if (flags && flags->dot && !flags->prcsn && !flags->width)
		return (n);
	if (flags->minus)
		n += ft_printstr(s, flags, len);
	while (flags->prcsn >= len && flags->width-- > len)
		n += write(1, " ", 1);
	while (len > flags->prcsn && flags->width-- > flags->prcsn)
		n += write(1, " ", 1);
	if (flags->minus)
		return (n);
	return (n += ft_printstr(s, flags, len));
}

int	str_empty_print(char *s, t_type *flags, int n)
{
	if (*s == '\0' && !flags)
		return (n);
	if (*s == '\0' && flags && flags->zero && flags->minus && !flags->width)
		return (n);
	else if (flags && flags->dot && flags->prcsn < 6 && !flags->width)
		return (n);
	else if ((flags && flags->width && flags->dot && !flags->prcsn)
		|| (flags && flags->width && s && *s == '\0')
		|| (flags && flags->space && s && *s == '\0'))
	{
		while (flags->width-- > 0)
			n += write (1, " ", 1);
		return (n);
	}
	return (n);
}

int	str_null_print(char *s, t_type *flags, int n)
{
	if (flags && flags->dot && flags->prcsn < 6 && !flags->width)
		return (n);
	else if (flags && flags->width && flags->dot && flags->prcsn < 6)
	{
		while (flags->width-- > 0)
			n += write (1, " ", 1);
		return (n);
	}
	else if (flags && flags->minus && flags->width)
	{
		n += write(1, "(null)", 6);
		while (flags && flags->width-- > 6)
			n += write(1, " ", 1);
		return (n);
	}
	while (flags && flags->width-- > 6)
		n += write(1, " ", 1);
	s = s + 0;
	return (n += write(1, "(null)", 6));
}
