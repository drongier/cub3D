/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 11:31:37 by mekundur          #+#    #+#             */
/*   Updated: 2023/12/11 11:31:51 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;
	size_t	slen;

	i = 0;
	sub = 0;
	slen = ft_strlen(s);
	if (start >= slen)
		len = 0;
	if (start < slen && (len + start) >= slen)
		len = slen - start;
	sub = (char *)malloc((len + 1) * sizeof(char));
	if (!sub || !s)
	{
		free(sub);
		return (0);
	}
	while (s[i] && i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}
