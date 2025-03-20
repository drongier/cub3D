/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isemptyline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:53:31 by mekundur          #+#    #+#             */
/*   Updated: 2025/03/04 16:59:29 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_is_emptyline(char *s)
{
	int	i;

	i = 0;
	while (s && s[i] && ft_is_whitespace(s[i]))
		i++;
	if (s[i] == '\n')
		return (1);
	return (0);
}
