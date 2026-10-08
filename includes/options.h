#ifndef OPTIONS_H
# define OPTIONS_H

# include <stdbool.h>

# define BENCH_DEFAULT_FRAMES 1000
# define BENCH_MAX_FRAMES 1000000

/* bench_frames == 0 : partie normale */
typedef struct s_options
{
	const char	*scene_path;
	bool		vsync;
	int			bench_frames;
}	t_options;

bool	parse_options(int argc, char **argv, t_options *opt);
void	print_usage(void);

#endif
