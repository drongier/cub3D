#include "../includes/raster.h"
#include <stdio.h>
#include <string.h>

static int	g_fail;

#define CHECK(cond, ...) do { if (!(cond)) { g_fail++; \
	printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); \
	printf("\n"); } } while (0)

#define SIZE 16

static uint32_t	g_px[SIZE * SIZE];

static t_canvas	canvas(int x0, int y0, int x1, int y1)
{
	memset(g_px, 0, sizeof(g_px));
	return ((t_canvas){g_px, SIZE, x0, y0, x1, y1});
}

static int	count(uint32_t color)
{
	int	n;
	int	i;

	n = 0;
	i = -1;
	while (++i < SIZE * SIZE)
		n += (g_px[i] == color);
	return (n);
}

int	main(void)
{
	t_canvas	c;
	t_vec2		sq[4] = {{2, 2}, {6, 2}, {6, 6}, {2, 6}};
	t_vec2		t1[3] = {{2, 2}, {10, 2}, {10, 10}};
	t_vec2		t2[3] = {{2, 2}, {10, 10}, {2, 10}};
	t_vec2		big[3] = {{-1e5f, -1e5f}, {1e5f, -1e5f}, {0, 1e5f}};

	CHECK(blend(0x000000, 0xFFFFFF, ALPHA_OPAQUE) == 0xFFFFFF, "opaque");
	CHECK(blend(0x123456, 0xFFFFFF, 0) == 0x123456, "transparent");
	CHECK(blend(0x000000, 0xFEFEFE, 128) == 0x7F7F7F, "half: %06X",
		blend(0x000000, 0xFEFEFE, 128));

	c = canvas(0, 0, SIZE, SIZE);
	fill_polygon(&c, sq, 4, 0xFF, ALPHA_OPAQUE);
	CHECK(count(0xFF) == 16 && g_px[2 * SIZE + 2] == 0xFF
		&& g_px[5 * SIZE + 5] == 0xFF && g_px[6 * SIZE + 6] == 0,
		"4x4 square covers exactly 16 pixels: %d", count(0xFF));

	c = canvas(4, 4, SIZE, SIZE);
	fill_polygon(&c, sq, 4, 0xFF, ALPHA_OPAQUE);
	CHECK(count(0xFF) == 4, "clipped square: %d", count(0xFF));

	c = canvas(0, 0, SIZE, SIZE);
	fill_polygon(&c, t1, 3, 0x010101, ALPHA_OPAQUE);
	fill_polygon(&c, t2, 3, 0x020202, ALPHA_OPAQUE);
	CHECK(count(0x010101) + count(0x020202) == 64,
		"two halves of a square cover it once: %d",
		count(0x010101) + count(0x020202));

	c = canvas(3, 3, 9, 9);
	fill_polygon(&c, big, 3, 0xFF, ALPHA_OPAQUE);
	CHECK(count(0xFF) == 36, "huge triangle fills the clip rect: %d",
		count(0xFF));

	c = canvas(0, 0, SIZE, SIZE);
	fill_span(&c, 3, -5, 100, 0xFF, ALPHA_OPAQUE);
	fill_span(&c, -1, 0, 4, 0xFF, ALPHA_OPAQUE);
	CHECK(count(0xFF) == SIZE, "span clipped to the row: %d", count(0xFF));
	if (g_fail)
		printf("raster_test: %d check(s) failed\n", g_fail);
	else
		printf("raster_test: ok\n");
	return (g_fail != 0);
}
