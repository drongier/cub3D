#include "../includes/cub3d.h"
#include <stdlib.h>
#include <math.h>
#include "mlx.h"

ButtonKeys Keys = {0, 0, 0, 0};

float degToRad(float a) { return a * M_PI / 180.0; }
float FixAng(float a) { if (a > 359) a -= 360; if (a < 0) a += 360; return a; }
float distance(float ax, float ay, float bx, float by, float ang) { return cos(degToRad(ang)) * (bx - ax) - sin(degToRad(ang)) * (by - ay); }

float px = 150, py = 400; // Player position
float pdx, pdy, pa = 90;  // Player direction and angle

int mapW[] = 
{
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 1, 0, 0, 0, 1,
    1, 0, 0, 4, 0, 2, 0, 1,
    1, 1, 0, 1, 0, 0, 0, 1,
    2, 0, 0, 0, 0, 0, 0, 1,
    2, 0, 0, 0, 0, 1, 0, 1,
    2, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 3, 1, 3, 1, 3, 1,
};

void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char *dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int *)dst = color;
}

void drawSquare(t_data *data, int x, int y, int size, int color)
{
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            my_mlx_pixel_put(data, x + i, y + j, color);
}

void drawLine(t_data *data, int x0, int y0, int x1, int y1, int color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1)
    {
        my_mlx_pixel_put(data, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void drawMap2D(t_data *data)
{
    for (int y = 0; y < mapY; y++)
    {
        for (int x = 0; x < mapX; x++)
        {
            int xo = x * mapS;
            int yo = y * mapS;
            if (mapW[y * mapX + x] > 0)
                drawSquare(data, xo, yo, mapS, 0xFFFFFF); // White for walls
            else
                drawSquare(data, xo, yo, mapS, 0x000000); // Black for empty space
        }
    }
}

void drawPlayer2D(t_data *data)
{
    drawSquare(data, px, py, 8, 0xFFFF00); // Yellow player
    drawLine(data, px, py, px + pdx * 20, py + pdy * 20, 0xFFFF00); // Player direction
}

int Buttons(int keycode)
{
    if (keycode == LEFT) // A key
    {
        pa += 5;
        pa = FixAng(pa);
        pdx = cos(degToRad(pa));
        pdy = -sin(degToRad(pa));
    }
    if (keycode == RIGHT) // D key
    {
        pa -= 5;
        pa = FixAng(pa);
        pdx = cos(degToRad(pa));
        pdy = -sin(degToRad(pa));
    }
    if (keycode == UP) // W key
    {
        px += pdx * 5;
        py += pdy * 5;
    }
    if (keycode == DOWN) // S key
    {
        px -= pdx * 5;
        py -= pdy * 5;
    }
	else if (keycode == 65307) // Touche Échap
		exit(0);
    return (0);
}

int render_frame(t_data *data)
{
    // Clear screen
    for (int y = 0; y < WINDOW_HEIGHT; y++)
        for (int x = 0; x < WINDOW_WIDTH; x++)
            my_mlx_pixel_put(data, x, y, 0x303030); // Gray background

    drawMap2D(data);
    drawPlayer2D(data);
    mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
    return (0);
}

int main(void)
{
    t_data data;

    data.mlx = mlx_init();
    data.win = mlx_new_window(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D - MinilibX");
    data.img = mlx_new_image(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel, &data.line_length, &data.endian);

    // Initialize player direction
    pdx = cos(degToRad(pa));
    pdy = -sin(degToRad(pa));

    // Hook events
    mlx_hook(data.win, 2, 1L << 0, Buttons, &data);
    mlx_loop_hook(data.mlx, render_frame, &data);

    // Main loop
    mlx_loop(data.mlx);

    return (0);
}