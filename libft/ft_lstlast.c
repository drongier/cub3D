/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekundur <mekundur@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 12:17:33 by mekundur          #+#    #+#             */
/*   Updated: 2023/12/11 12:17:40 by mekundur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	int		size;
	t_list	*ptr;

	size = 0;
	ptr = lst;
	while (ptr != 0)
	{
		ptr = ptr->next;
		size++;
	}
	ptr = lst;
	while (size - 1 > 0)
	{
		ptr = ptr->next;
		size--;
	}
	return (ptr);
}
