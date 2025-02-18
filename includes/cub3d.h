/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 12:01:38 by drongier          #+#    #+#             */
/*   Updated: 2025/02/18 18:10:46 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

#include <mlx.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../libft/libft.h"

#define WINDOW_WIDTH  510
#define WINDOW_HEIGHT 510

typedef struct {
    int w, a, d, s; // Button states
} ButtonKeys;


#define SQUARE_SIZE 20
#define BUFFER_SIZE 28
#define MAP_WIDTH 4
#define MAP_HEIGHT 4
#define mapX  8      // map width
#define mapY  8      // map height
#define mapS 64      // map cube size
#define M_PI 3.1428
#define LEFT 65361
#define RIGHT 65363
#define UP 65362
#define DOWN 65364

typedef struct s_data {
    void *mlx;
    void *win;
    void *img;
    char *addr;
    int bits_per_pixel;
    int line_length;
    int endian;
} t_data;

#endif
