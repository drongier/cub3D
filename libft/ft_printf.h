/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:57:35 by mekundur          #+#    #+#             */
/*   Updated: 2024/02/19 06:39:51 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stddef.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <limits.h>

typedef struct s_flag
{
	int	space;
	int	plus;
	int	hash;
	int	minus;
	int	zero;
	int	width;
	int	dot;
	int	prcsn;
}	t_type;

int	ft_printf(const char *format, ...);
int	set_iteration(const char *format, t_type *flags);
int	ft_putchar(int c, t_type *flags);
int	ft_putstr(char *s, t_type *flags);
int	ft_printstr(char *s, t_type *flags, int len);
int	str_null_print(char *s, t_type *flags, int n);
int	str_empty_print(char *s, t_type *flags, int n);
int	strflag_width_prcsn(char *s, t_type *flags, int len);
int	ft_putnbr(long long n, t_type *flags);
int	nbrflag_search(long long num, long long *ptr, t_type *flags, int n);
int	nbrflag_minus(long long num, long long *ptr, t_type *flags, int n);
int	printnbr(long long num, long long *ptr, t_type *flags);
int	ft_diglen(long long num);
int	nbrflag_prcsn(long long num, long long *ptr, t_type *flags, int n);
int	nbrflag_space_width(long long num, long long *ptr, t_type *flags, int n);
int	nbrflag_plus_width(long long num, long long *ptr, t_type *flags, int n);
int	nbrflag_space_zero(long long num, long long *ptr, t_type *flags, int n);
int	ft_puthex(unsigned long long num, const char *format, t_type *flags);
int	ft_printhex(unsigned long long num, const char *format);
int	ft_putptr(unsigned long long num, const char *format, t_type *flags);
int	ft_hex(unsigned long long num, const char *format);
int	hexlen(unsigned long long num);
int	hexfl_sch(unsigned long long num, const char *format, t_type *flags, int n);
int	hexfl_pre(unsigned long long num, const char *format, t_type *flags, int n);
int	hexfl_min(unsigned long long num, const char *format, t_type *flags, int n);
int	hexfl_hash(const char *format);
int	ft_flaghex_width(unsigned long long num, t_type *flags, int len);
int	hex_null_print(int n, t_type *flags);

#endif
