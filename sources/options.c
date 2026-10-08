#include "../includes/options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool	is_number(const char *s)
{
	if (!*s)
		return (false);
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (false);
		s++;
	}
	return (true);
}

/* --bench [N] : N est pris seulement si l'argument suivant est un nombre */
static bool	parse_bench(int argc, char **argv, int *i, t_options *opt)
{
	long	n;

	opt->bench_frames = BENCH_DEFAULT_FRAMES;
	if (*i + 1 >= argc || !is_number(argv[*i + 1]))
		return (true);
	(*i)++;
	if (strlen(argv[*i]) > 7)
		return (false);
	n = strtol(argv[*i], NULL, 10);
	if (n <= 0 || n > BENCH_MAX_FRAMES)
		return (false);
	opt->bench_frames = (int)n;
	return (true);
}

/* --fps N : N obligatoire, entre FPS_CAP_MIN et FPS_CAP_MAX */
static bool	parse_fps(int argc, char **argv, int *i, t_options *opt)
{
	long	n;

	if (*i + 1 >= argc || !is_number(argv[*i + 1]) || strlen(argv[*i + 1]) > 4)
		return (false);
	(*i)++;
	n = strtol(argv[*i], NULL, 10);
	if (n < FPS_CAP_MIN || n > FPS_CAP_MAX)
		return (false);
	opt->fps_cap = (int)n;
	return (true);
}

bool	parse_options(int argc, char **argv, t_options *opt)
{
	int	i;

	opt->scene_path = NULL;
	opt->vsync = true;
	opt->bench_frames = 0;
	opt->fps_cap = 0;
	i = 1;
	while (i < argc)
	{
		if (strcmp(argv[i], "--no-vsync") == 0)
			opt->vsync = false;
		else if (strcmp(argv[i], "--bench") == 0)
		{
			if (!parse_bench(argc, argv, &i, opt))
				return (false);
		}
		else if (strcmp(argv[i], "--fps") == 0)
		{
			if (!parse_fps(argc, argv, &i, opt))
				return (false);
		}
		else if (argv[i][0] == '-' || opt->scene_path)
			return (false);
		else
			opt->scene_path = argv[i];
		i++;
	}
	if (opt->bench_frames > 0)
	{
		opt->vsync = false;
		opt->fps_cap = 0;
	}
	return (opt->scene_path != NULL);
}

void	print_usage(void)
{
	fprintf(stderr, "Usage: ./cub3D [--no-vsync] [--fps N] [--bench [N]] <scene.cub>\n");
}
