#include "../includes/cub3d.h"

typedef struct s_fps_window
{
	uint64_t	start;
	int			frames;
	double		frame_ms;
	double		render_ms;
}	t_fps_window;

static void	apply_input(t_player *player, const t_input *in)
{
	player->key_up = in->up;
	player->key_down = in->down;
	player->key_left = in->left;
	player->key_right = in->right;
	player->left_rotate = in->rot_left;
	player->right_rotate = in->rot_right;
}

/* Titre mis à jour toutes les 500 ms avec les moyennes de la fenêtre */
static void	update_title(t_platform *p, t_fps_window *w, double frame_ms,
		double render_ms)
{
	char	title[128];
	double	elapsed;

	w->frames++;
	w->frame_ms += frame_ms;
	w->render_ms += render_ms;
	elapsed = platform_ticks_to_ms(platform_ticks() - w->start);
	if (elapsed < 500.0)
		return ;
	snprintf(title, sizeof(title),
		"cub3D | %.0f fps | frame %.1f ms | render %.1f ms",
		w->frames * 1000.0 / elapsed, w->frame_ms / w->frames,
		w->render_ms / w->frames);
	platform_set_title(p, title);
	w->start = platform_ticks();
	w->frames = 0;
	w->frame_ms = 0;
	w->render_ms = 0;
}

static void	print_row(const char *name, t_stats_summary s)
{
	printf("%-8s %-7.2f %-7.2f %-7.2f %-7.2f %-7.2f\n",
		name, s.avg, s.median, s.p99, s.min, s.max);
}

static void	print_report(const t_options *opt, double *render, double *frame,
		int n)
{
	t_stats_summary	r;
	t_stats_summary	f;

	r = stats_summarize(render, n);
	f = stats_summarize(frame, n);
	printf("bench: %s, %d frames, %dx%d, vsync off\n",
		opt->scene_path, n, WIDTH, HEIGHT);
	printf("%-8s %-7s %-7s %-7s %-7s %-7s (ms)\n",
		"", "avg", "median", "p99", "min", "max");
	print_row("render", r);
	print_row("frame", f);
	if (f.avg > 0)
		printf("fps (avg frame): %.1f\n", 1000.0 / f.avg);
}

/* Bench : le clavier est ignoré sauf pour quitter, le joueur tourne sur place */
static void	frame_input(t_game *game, const t_options *opt, const t_input *in)
{
	t_input	scripted;

	if (opt->bench_frames == 0)
	{
		apply_input(&game->player, in);
		return ;
	}
	ft_bzero(&scripted, sizeof(scripted));
	scripted.rot_right = true;
	apply_input(&game->player, &scripted);
}

/* Temps écoulé depuis la frame précédente, fixe en bench */
static double	frame_dt(const t_options *opt, uint64_t *last, uint64_t now)
{
	double	dt;

	dt = platform_ticks_to_ms(now - *last) / 1000.0;
	*last = now;
	if (opt->bench_frames > 0)
		return (MOTION_BENCH_DT);
	return (motion_clamp_dt(dt));
}

/* --fps N : attend la fin du créneau de 1/N s commencé en début de frame */
static void	cap_fps(const t_options *opt, uint64_t frame_start)
{
	double	left_ms;

	if (opt->fps_cap == 0)
		return ;
	left_ms = 1000.0 / opt->fps_cap
		- platform_ticks_to_ms(platform_ticks() - frame_start);
	if (left_ms > 0)
		platform_sleep_ns((uint64_t)(left_ms * 1e6));
}

int	run_loop(t_game *game, const t_options *opt)
{
	t_input			input;
	t_fps_window	win;
	double			*render;
	double			*frame;
	uint64_t		t[4];
	uint64_t		last;
	int				n;

	render = NULL;
	frame = NULL;
	if (opt->bench_frames > 0)
	{
		render = malloc(sizeof(double) * opt->bench_frames);
		frame = malloc(sizeof(double) * opt->bench_frames);
		if (!render || !frame)
		{
			free(render);
			free(frame);
			printf("Error: Allocation error!\n");
			return (1);
		}
	}
	ft_bzero(&input, sizeof(input));
	ft_bzero(&win, sizeof(win));
	win.start = platform_ticks();
	last = win.start;
	n = 0;
	while (opt->bench_frames == 0 || n < opt->bench_frames)
	{
		t[0] = platform_ticks();
		platform_poll(game->platform, &input);
		if (input.quit)
			break ;
		frame_input(game, opt, &input);
		update_player(&game->player, frame_dt(opt, &last, t[0]));
		t[1] = platform_ticks();
		draw_loop(game);
		t[2] = platform_ticks();
		platform_present(game->platform, game->fb);
		t[3] = platform_ticks();
		if (render)
		{
			render[n] = platform_ticks_to_ms(t[2] - t[1]);
			frame[n] = platform_ticks_to_ms(t[3] - t[0]);
		}
		n++;
		update_title(game->platform, &win, platform_ticks_to_ms(t[3] - t[0]),
			platform_ticks_to_ms(t[2] - t[1]));
		cap_fps(opt, t[0]);
	}
	if (render && n > 0)
		print_report(opt, render, frame, n);
	free(render);
	free(frame);
	return (0);
}
