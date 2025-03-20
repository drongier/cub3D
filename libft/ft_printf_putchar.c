/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 18:01:35 by mekundur          #+#    #+#             */
/*   Updated: 2024/02/18 22:45:54 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int	ft_putchar(int c, t_type *flags)
{
	int	n;

	n = 0;
	if (flags)
	{
		if (flags->minus == 1)
		{
			n += write(1, &c, 1);
			while (--flags->width > 0)
				n += write(1, " ", 1);
		}
		else if (flags->width)
		{
			while ((flags->width--) - 1 > 0)
				n += write(1, " ", 1);
			n += write(1, &c, 1);
		}
		else if (flags->dot)
			n += write(1, &c, 1);
	}
	else
		n += write(1, &c, 1);
	return (n);
}
