/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isemptyline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:53:31 by mekundur          #+#    #+#             */
/*   Updated: 2025/05/12 13:15:20 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_is_emptyline(char *s)
{
	int	i;

	if (!s)
		return (1);
	i = 0;
	while (s[i] && ft_is_whitespace(s[i]))
		i++;
	return (s[i] == '\n' || s[i] == '\0');
}
