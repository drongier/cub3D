/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:56:58 by mekundur          #+#    #+#             */
/*   Updated: 2024/02/19 06:40:15 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static int	conversion(va_list ap, const char *format, t_type *flags)
{
	int		n;

	n = 0;
	if (*format == '%')
		n += write(1, "%", 1);
	else if (*format == 'c')
		n += ft_putchar(va_arg(ap, int), flags);
	else if (*format == 'd' || *format == 'i')
		n += ft_putnbr(va_arg(ap, int), flags);
	else if (*format == 'u')
		n += ft_putnbr(va_arg(ap, unsigned int), flags);
	else if (*format == 's')
		n += ft_putstr(va_arg(ap, char *), flags);
	else if (*format == 'x' || *format == 'X')
		n += ft_puthex(va_arg(ap, unsigned int), format, flags);
	else if (*format == 'p')
		n += ft_putptr(va_arg(ap, unsigned long), format, flags);
	return (n);
}

static int	ft_flaglen(const char *format)
{
	int	i;

	i = 0;
	while (format[i])
	{
		if (format[i] == 'd' || format[i] == 'i' || format[i] == 'u'
			|| format[i] == 'x' || format[i] == 'X'
			|| format[i] == 'c' || format[i] == 's'
			|| format[i] == 'p' || format[i] == '%')
			break ;
		i++;
	}
	return (i);
}

static void	flag_search(const char *format, t_type *flags, int amount, int i)
{
	while (amount > i)
	{
		if (format[i] == ' ')
			flags->space = 1;
		else if (format[i] == '+')
			flags->plus = 1;
		else if (format[i] == '#')
			flags->hash = 2;
		else if (format[i] == '-')
			flags->minus = 1;
		else if (format[i] == '0')
			flags->zero = 1;
		else if (format[i] >= '0' && format[i] <= '9')
		{
			flags->width = ft_atoi(&format[i]);
			i += ft_diglen(flags->width) - 1;
		}
		else if (format[i] == '.')
		{
			flags->dot = 1;
			flags->prcsn = ft_atoi(&format[++i]);
			i += set_iteration(format++, flags);
		}
		i++;
	}
}

static int	flag_init(va_list ap, const char *format, t_type *flags)
{
	int	n;
	int	i;
	int	amount;

	i = 0;
	n = 0;
	amount = ft_flaglen(format);
	if (amount > 0)
	{
		flags = (t_type *)malloc(sizeof(t_type));
		if (!flags)
			return (0);
		flags->space = 0;
		flags->plus = 0;
		flags->hash = 0;
		flags->minus = 0;
		flags->zero = 0;
		flags->dot = 0;
		flags->prcsn = 0;
		flags->width = 0;
		flag_search(format, flags, amount, i);
	}
	n += conversion(ap, format + amount, flags);
	return (free(flags), n);
}

int	ft_printf(const char *format, ...)
{
	int		n;
	va_list	ap;
	t_type	*flags;

	n = 0;
	flags = NULL;
	va_start(ap, format);
	while (*format != 0)
	{
		if (*format == '%')
		{
			format++;
			if (*format == '%')
				n += write(1, "%", 1);
			else
			{
				n += flag_init(ap, format, flags);
				format += ft_flaglen(format);
			}
		}
		else
			n += write(1, &(*format), 1);
		format++;
	}
	return (va_end(ap), n);
}
