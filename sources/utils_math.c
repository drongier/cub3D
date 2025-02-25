/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:30:33 by drongier          #+#    #+#             */
/*   Updated: 2025/02/25 17:31:08 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

// distance calculation functions
float distance(float x, float y)
{
    return sqrt(x * x + y * y);
}

//Removing distortion + fisheyes effect 
float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
{
    float delta_x = x2 - x1;
    float delta_y = y2 - y1;
    float angle = atan2(delta_y, delta_x) - game->player.angle;
    float fix_dist = distance(delta_x, delta_y) * cos(angle);
    return fix_dist;
}