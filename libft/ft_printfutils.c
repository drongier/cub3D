/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printfutils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 18:03:08 by mekundur          #+#    #+#             */
/*   Updated: 2024/02/19 02:34:07 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int	set_iteration(const char *format, t_type *flags)
{
	int	i;

	i = 0;
	while (format[i] >= '0' && format[i] <= '9')
		++i;
	return (i + 1);
	flags = flags + 0;
}

int	ft_diglen(long long num)
{
	int	len;

	len = 0;
	if (num <= 0)
	{
		len++;
		num *= -1;
	}
	while (num >= 1)
	{
		num = num / 10;
		len++;
	}
	return (len);
}
