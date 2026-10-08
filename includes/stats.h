#ifndef STATS_H
# define STATS_H

typedef struct s_stats_summary
{
	double	avg;
	double	median;
	double	p99;
	double	min;
	double	max;
}	t_stats_summary;

/* Trie samples sur place. n <= 0 renvoie des zéros. */
t_stats_summary	stats_summarize(double *samples, int n);

#endif
