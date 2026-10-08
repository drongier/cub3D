#include "../includes/stats.h"
#include <math.h>
#include <stdio.h>

static int	g_fail;

#define CHECK(cond, ...) do { if (!(cond)) { g_fail++; \
	printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); \
	printf("\n"); } } while (0)

static int	near(double a, double b)
{
	return (fabs(a - b) < 1e-9);
}

int	main(void)
{
	double			one[] = {5.0};
	double			four[] = {4.0, 1.0, 3.0, 2.0};
	double			big[1000];
	t_stats_summary	s;
	int				i;

	s = stats_summarize(one, 1);
	CHECK(near(s.avg, 5) && near(s.median, 5) && near(s.p99, 5)
		&& near(s.min, 5) && near(s.max, 5), "single sample");
	s = stats_summarize(four, 4);
	CHECK(near(s.avg, 2.5), "avg4 %f", s.avg);
	CHECK(near(s.median, 2.5), "median4 %f", s.median);
	CHECK(near(s.min, 1) && near(s.max, 4), "min/max4");
	CHECK(near(s.p99, 4), "p99 of 4 = %f", s.p99);
	i = 0;
	while (i < 1000)
	{
		big[i] = 1000.0 - i;
		i++;
	}
	s = stats_summarize(big, 1000);
	CHECK(near(s.p99, 990), "p99 of 1..1000 = %f", s.p99);
	CHECK(near(s.median, 500.5), "median of 1..1000 = %f", s.median);
	CHECK(near(s.avg, 500.5), "avg of 1..1000 = %f", s.avg);
	s = stats_summarize(NULL, 0);
	CHECK(near(s.avg, 0) && near(s.max, 0), "empty input gives zeros");
	if (g_fail)
		printf("stats_test: %d check(s) failed\n", g_fail);
	else
		printf("stats_test: ok\n");
	return (g_fail != 0);
}
