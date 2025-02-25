/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drongier <drongier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:01:36 by drongier          #+#    #+#             */
/*   Updated: 2025/02/25 14:21:38 by drongier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	put_pixel(int x, int y, int color, t_game *game)
{
	int	index;
	
	if(x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return;
	index = y * game->size_line + x * game->bpp / 8;
	game->data[index] = color & 0xFF; 				//BLUE COMPOSANTE 
	game->data[index + 1] = (color >> 8) & 0xFF; 	// GREEN COMPOSANTE
	game->data[index + 2] = (color >> 16) & 0xFF; 	// RED COMPOSTANTE
}

// raycasting functions
void draw_line(t_player *player, t_game *game, float start_x, int i)
{
    float cos_angle = cos(start_x); //axe X
    float sin_angle = sin(start_x); //axe Y 
    float ray_x = player->x;
    float ray_y = player->y;

    while(!touch(ray_x, ray_y, game))
    {
        if(DEBUG)
            put_pixel(ray_x, ray_y, 0xFF0000, game); //color vision fps ici red
        ray_x += cos_angle;
        ray_y += sin_angle;
    }
    if(!DEBUG)
    {
        float dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
        float height = (BLOCK / dist) * (WIDTH / 2);
        int start_y = (HEIGHT - height) / 2;
        int end = start_y + height;
		
		int j = 0;
		while (j < start_y)
			put_pixel(i, j++, 0x74583A, game); //wall
		while(start_y < end)
        {
            put_pixel(i, start_y, 0x11C7C7, game); //celling
            start_y++;
        }
		j = start_y;
		while (j < HEIGHT)
			put_pixel(i, j++, 0x999999, game); //ground
    }
}
void draw_square_full(int x, int y, int size, int color, t_game *game)
{
    int i = 0;
    while (i < size)
    {
        int j = 0;
        while (j < size)
        {
            put_pixel(x + i, y + j, color, game);
            j++;
        }
        i++;
    }
}

/* GRAPHIC ENGINE */
int draw_loop(t_game *game)
{
    t_player *player = &game->player;
    move_player(player);
    clear_image(game);
    if(DEBUG)
    {
        draw_square(player->x, player->y, 10, 0x00FF00, game); //small square for player, ici green
        //draw_map(game);
    }
    /* PI/3 = 60° = Player vision like FPS game then /WIDTH to equal sample for raycasting */
    float fraction = PI / 3 / WIDTH;

    /*   FOV left <-- [angle du joueur] --> FOV right
         start_x     <-- 30° --> centre <-- 30° --> end */
    float start_x = player->angle - PI / 6;
    int i = 0;

    /*    Joueur
            |  [Rays]
            |    /
            |   /  ← Angle augmente graduellement (fraction + 1)
            |  /   ← Chaque rays = une colonne à l'écran
            | /    ← from start_x to end
            |/                                                */
    while(i < WIDTH)
    {
        draw_line(player, game, start_x, i);
        start_x += fraction;
        i++;
    }
	draw_target(game);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
    return 0;
}