/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 18:01:11 by mekundur          #+#    #+#             */
/*   Updated: 2024/02/18 22:57:13 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int	ft_printstr(char *s, t_type *flags, int len)
{
	int	n;

	n = 0;
	if (flags && flags->dot && flags->prcsn < len)
	{
		while (flags->prcsn > n)
			n += write(1, &s[n], 1);
	}
	else
	{
		while (s[n])
			n += write(1, &s[n], 1);
	}
	return (n);
}

int	ft_putstr(char *s, t_type *flags)
{
	int	len;
	int	n;

	n = 0;
	if (s == NULL)
		return (str_null_print(s, flags, 0));
	if (*s == '\0')
		return (str_empty_print(s, flags, 0));
	len = ft_strlen(s);
	if (flags && flags->dot && !flags->prcsn && !flags->width)
		return (n);
	else if (flags && flags->width && flags->prcsn)
		return (n += strflag_width_prcsn(s, flags, len));
	else if (flags && flags->prcsn && !flags->width)
		return (ft_printstr(s, flags, (int)len));
	else if (flags && flags->minus)
		n += ft_printstr(s, flags, (int)len);
	while (flags && flags->dot && !flags->prcsn && flags->width-- > 0)
		n += write(1, " ", 1);
	while (flags && flags->width-- > (int)len && flags->width > 0)
		n += write (1, " ", 1);
	if (flags && flags->minus)
		return (n);
	else
		return (n += ft_printstr(s, flags, (int)len));
}
