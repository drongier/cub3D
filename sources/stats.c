#include "../includes/stats.h"
#include <stdlib.h>
#include <string.h>

static int	cmp_double(const void *a, const void *b)
{
	double	x;
	double	y;

	x = *(const double *)a;
	y = *(const double *)b;
	return ((x > y) - (x < y));
}

t_stats_summary	stats_summarize(double *samples, int n)
{
	t_stats_summary	s;
	double			sum;
	int				i;

	memset(&s, 0, sizeof(s));
	if (n <= 0)
		return (s);
	qsort(samples, (size_t)n, sizeof(double), cmp_double);
	sum = 0;
	i = 0;
	while (i < n)
		sum += samples[i++];
	s.avg = sum / n;
	s.min = samples[0];
	s.max = samples[n - 1];
	if (n % 2)
		s.median = samples[n / 2];
	else
		s.median = (samples[n / 2 - 1] + samples[n / 2]) / 2.0;
	s.p99 = samples[(99L * n + 99) / 100 - 1];
	return (s);
}
