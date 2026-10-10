#include "../includes/cub3d.h"
#ifdef __EMSCRIPTEN__
# include <emscripten.h>
#endif

typedef struct s_fps_window
{
	uint64_t	start;
	int			frames;
	double		frame_ms;
	double		render_ms;
}	t_fps_window;

typedef struct s_loop
{
	t_game			*game;
	const t_options	*opt;
	t_input			input;
	t_fps_window	win;
	double			*render;
	double			*frame;
	uint64_t		last;
	int				n;
}	t_loop;

static void	apply_input(t_player *player, const t_input *in)
{
	player->key_up = in->up;
	player->key_down = in->down;
	player->key_left = in->left;
	player->key_right = in->right;
	player->left_rotate = in->rot_left;
	player->right_rotate = in->rot_right;
	player->sprint = in->sprint;
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
		game->trigger = in->fire;
		return ;
	}
	game->trigger = false;
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

/* Une frame : entrées, mise à jour, dessin, affichage ; false pour arrêter */
static bool	loop_frame(t_loop *l)
{
	uint64_t	t[4];
	double		dt;

	t[0] = platform_ticks();
	platform_poll(l->game->platform, &l->input);
	if (l->input.quit)
		return (false);
	frame_input(l->game, l->opt, &l->input);
	dt = frame_dt(l->opt, &l->last, t[0]);
	if (!game_update(l->game, dt))
		return (false);
	t[1] = platform_ticks();
	draw_loop(l->game);
	t[2] = platform_ticks();
	platform_present(l->game->platform, l->game->fb);
	t[3] = platform_ticks();
	if (l->render)
	{
		l->render[l->n] = platform_ticks_to_ms(t[2] - t[1]);
		l->frame[l->n] = platform_ticks_to_ms(t[3] - t[0]);
	}
	l->n++;
	update_title(l->game->platform, &l->win, platform_ticks_to_ms(t[3] - t[0]),
		platform_ticks_to_ms(t[2] - t[1]));
	cap_fps(l->opt, t[0]);
	return (l->opt->bench_frames == 0 || l->n < l->opt->bench_frames);
}

static int	loop_end(t_loop *l)
{
	if (l->render && l->n > 0)
		print_report(l->opt, l->render, l->frame, l->n);
	free(l->render);
	free(l->frame);
	return (0);
}

#ifdef __EMSCRIPTEN__

/*
 * Navigateur : pas de boucle bloquante, il appelle web_frame à chaque
 * rafraîchissement de l'écran. Quand le jeu s'arrête, tout est libéré.
 */
static void	web_frame(void *arg)
{
	t_loop	*l;

	l = arg;
	if (loop_frame(l))
		return ;
	emscripten_cancel_main_loop();
	loop_end(l);
	game_destroy(l->game);
}

static int	loop_run(t_loop *l)
{
	static t_loop	kept;

	kept = *l;
	emscripten_set_main_loop_arg(web_frame, &kept, 0, false);
	return (-1);
}

#else

static int	loop_run(t_loop *l)
{
	while (loop_frame(l))
		;
	return (loop_end(l));
}

#endif

/* Renvoie -1 quand la boucle continue sans nous (navigateur) */
int	run_loop(t_game *game, const t_options *opt)
{
	t_loop	l;

	ft_bzero(&l, sizeof(l));
	l.game = game;
	l.opt = opt;
	if (opt->bench_frames > 0)
	{
		l.render = malloc(sizeof(double) * opt->bench_frames);
		l.frame = malloc(sizeof(double) * opt->bench_frames);
		if (!l.render || !l.frame)
		{
			free(l.render);
			free(l.frame);
			printf("Error: Allocation error!\n");
			return (1);
		}
	}
	l.win.start = platform_ticks();
	l.last = l.win.start;
	return (loop_run(&l));
}
