/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iswhitespace.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:52:25 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/12 13:16:36 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_is_whitespace(char s)
{
	char	*whitespace;

	whitespace = " \t\r\v\f";
	if (ft_strchr(whitespace, s))
		return (1);
	return (0);
}
