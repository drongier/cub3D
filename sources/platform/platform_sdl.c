#include "../../includes/platform.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>

struct s_platform
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
	int				width;
	int				height;
};

static t_platform	*platform_fail(t_platform *p, const char *what)
{
	fprintf(stderr, "SDL: %s failed: %s\n", what, SDL_GetError());
	platform_destroy(p);
	return (NULL);
}

t_platform	*platform_init(int width, int height, bool vsync)
{
	t_platform	*p;

	p = calloc(1, sizeof(*p));
	if (!p)
		return (NULL);
	p->width = width;
	p->height = height;
	if (!SDL_Init(SDL_INIT_VIDEO))
		return (platform_fail(p, "SDL_Init"));
	p->window = SDL_CreateWindow("cub3D", width, height, 0);
	if (!p->window)
		return (platform_fail(p, "SDL_CreateWindow"));
	p->renderer = SDL_CreateRenderer(p->window, NULL);
	if (!p->renderer)
		return (platform_fail(p, "SDL_CreateRenderer"));
	if (!SDL_SetRenderVSync(p->renderer, vsync ? 1 : SDL_RENDERER_VSYNC_DISABLED))
		fprintf(stderr, "SDL: vsync setting ignored: %s\n", SDL_GetError());
	p->texture = SDL_CreateTexture(p->renderer, SDL_PIXELFORMAT_XRGB8888,
			SDL_TEXTUREACCESS_STREAMING, width, height);
	if (!p->texture)
		return (platform_fail(p, "SDL_CreateTexture"));
	SDL_SetTextureScaleMode(p->texture, SDL_SCALEMODE_NEAREST);
	return (p);
}

/* Position physique des touches : W/A/S/D en QWERTY = Z/Q/S/D en AZERTY */
static void	set_key(t_input *in, SDL_Scancode sc, bool down)
{
	if (sc == SDL_SCANCODE_W)
		in->up = down;
	else if (sc == SDL_SCANCODE_S)
		in->down = down;
	else if (sc == SDL_SCANCODE_A)
		in->left = down;
	else if (sc == SDL_SCANCODE_D)
		in->right = down;
	else if (sc == SDL_SCANCODE_LEFT)
		in->rot_left = down;
	else if (sc == SDL_SCANCODE_RIGHT)
		in->rot_right = down;

	else if (sc == SDL_SCANCODE_ESCAPE && down)
		in->quit = true;
}

void	platform_poll(t_platform *p, t_input *in)
{
	SDL_Event	e;
	const bool	*keys;

	(void)p;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT
			|| e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
			in->quit = true;
		else if ((e.type == SDL_EVENT_KEY_DOWN || e.type == SDL_EVENT_KEY_UP)
			&& !e.key.repeat)
			set_key(in, e.key.scancode, e.type == SDL_EVENT_KEY_DOWN);
	}
	keys = SDL_GetKeyboardState(NULL);
	in->sprint = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
	in->fire = keys[SDL_SCANCODE_SPACE]
		|| (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK);
}

void	platform_present(t_platform *p, const uint32_t *fb)
{
	SDL_UpdateTexture(p->texture, NULL, fb, p->width * (int)sizeof(uint32_t));
	SDL_RenderTexture(p->renderer, p->texture, NULL, NULL);
	SDL_RenderPresent(p->renderer);
}

void	platform_set_title(t_platform *p, const char *title)
{
	SDL_SetWindowTitle(p->window, title);
}

uint64_t	platform_ticks(void)
{
	return (SDL_GetPerformanceCounter());
}

void	platform_sleep_ns(uint64_t ns)
{
	SDL_DelayPrecise(ns);
}

double	platform_ticks_to_ms(uint64_t ticks)
{
	return ((double)ticks * 1000.0 / (double)SDL_GetPerformanceFrequency());
}

void	platform_destroy(t_platform *p)
{
	if (!p)
		return ;
	if (p->texture)
		SDL_DestroyTexture(p->texture);
	if (p->renderer)
		SDL_DestroyRenderer(p->renderer);
	if (p->window)
		SDL_DestroyWindow(p->window);
	SDL_Quit();
	free(p);
}
