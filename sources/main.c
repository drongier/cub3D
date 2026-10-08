#include "../includes/cub3d.h"

static int	fail(t_game *game, const char *msg)
{
	printf("Error\n%s\n", msg);
	game_destroy(game);
	return (1);
}

int	main(int argc, char **argv)
{
	t_game			game;
	t_options		opt;
	t_level_error	err;
	int				status;

	if (!parse_options(argc, argv, &opt))
		return (print_usage(), 1);
	if (!game_init(&game))
		return (fail(&game, "out of memory"));
	if (!game_load_level(&game, opt.scene_path, &err))
	{
		level_print_error(&err);
		game_destroy(&game);
		return (1);
	}
	if (opt.check)
	{
		printf("OK %s (%d x %d)\n", opt.scene_path, game.level.w,
			game.level.h);
		game_destroy(&game);
		return (0);
	}
	game.platform = platform_init(WIDTH, HEIGHT, opt.vsync);
	if (!game.platform)
		return (fail(&game, "window initialisation failed"));
	status = run_loop(&game, &opt);
	game_destroy(&game);
	return (status);
}
