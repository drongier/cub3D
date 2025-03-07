/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:22:08 by drongier          #+#    #+#             */
/*   Updated: 2025/03/07 14:51:04 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	is_west_east(float start_x)
{
	if (cos(start_x) > 0)
		return (WEST);
	else
		return (EAST);
}

int	is_nord_sud(float start_x)
{
	if (sin(start_x) > 0)
		return (SUD);
	else
		return (NORD);
}

int get_hit_direction(float start_x, int is_vertical)
{
    if (is_vertical)
    {
        if (cos(start_x) > 0)
            return WEST;
        else
            return EAST;
    }
    else
    {
        if (sin(start_x) > 0)
            return SUD;
        else
            return NORD;
    }
}