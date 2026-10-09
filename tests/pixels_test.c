#include "../includes/pixels.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int	g_fail;

#define CHECK(cond, ...) do { if (!(cond)) { g_fail++; \
	printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); \
	printf("\n"); } } while (0)

#define W 3
#define H 50

/* Référence : la formule de l'ancien draw_wall, pixel par pixel */
static void	reference(uint32_t *fb, int x, t_wall_span s, const uint32_t *col,
		int th)
{
	int	y;

	y = s.start_y;
	if (y < 0)
		y = 0;
	while (y < s.start_y + s.height && y < s.screen_h)
	{
		fb[y * s.stride + x] = col[(long)(y - s.start_y) * th / s.height];
		y++;
	}
}

static void	column_cases(void)
{
	uint32_t	col[64];
	uint32_t	a[W * H];
	uint32_t	b[W * H];
	int			th;
	int			start;
	int			h;
	int			bad;

	bad = 0;
	for (int i = 0; i < 64; i++)
		col[i] = 0x10000 + i;
	for (th = 1; th <= 64; th += 7)
		for (h = 1; h <= 400; h += (h < 60 ? 1 : 37))
			for (start = -350; start <= H; start += 13)
			{
				memset(a, 0, sizeof(a));
				memset(b, 0, sizeof(b));
				reference(a, 1, (t_wall_span){start, h, H, W}, col, th);
				draw_tex_column(b + 1, &(t_wall_span){start, h, H, W}, col, th);
				bad += memcmp(a, b, sizeof(a)) != 0;
			}
	CHECK(bad == 0, "column matches the reference formula (%d cases off)", bad);
}

/* 2 x 2 texture, pixel (1, 0) transparent, agrandie 2 fois */
static void	blit_cases(void)
{
	uint32_t	px[4] = {0xA, TEX_TRANSPARENT, 0xC, 0xD};
	uint32_t	fb[W * H];
	t_texture	t;

	t.data = (char *)px;
	t.width = 2;
	t.height = 2;
	for (int i = 0; i < W * H; i++)
		fb[i] = 7;
	blit_sprite(fb, W, H, &t, (t_blit){-1, H - 2, 2});
	CHECK(fb[(H - 2) * W + 0] == 0xA && fb[(H - 2) * W + 1] == 7
		&& fb[(H - 2) * W + 2] == 7 && fb[(H - 1) * W + 0] == 0xA,
		"top row: left texel clipped to one column, transparent skipped");
	CHECK(fb[(H - 3) * W + 0] == 7 && fb[(H - 3) * W + 1] == 7,
		"nothing above the sprite");
	blit_sprite(fb, W, H, &t, (t_blit){0, H - 1, 2});
	CHECK(fb[(H - 1) * W + 0] == 0xA && fb[(H - 1) * W + 1] == 0xA,
		"rows below the screen are clipped");
}

int	main(void)
{
	uint32_t	fb[W * H];
	uint32_t	px[6] = {1, 2, 3, 4, 5, 6};
	t_texture	t;

	memset(fb, 0, sizeof(fb));
	fill_rows(fb, W, 2, 4, 0xABCDEF);
	CHECK(fb[W * 2] == 0xABCDEF && fb[W * 4 - 1] == 0xABCDEF && fb[W * 4] == 0
		&& fb[W * 2 - 1] == 0, "fill_rows writes exactly rows 2 and 3");
	column_cases();

	t.data = malloc(sizeof(px));
	memcpy(t.data, px, sizeof(px));
	t.width = 3;
	t.height = 2;
	t.bpp = 32;
	t.size_line = 12;
	t.columns = NULL;
	CHECK(texture_build_columns(&t), "build columns");
	CHECK(texture_column(&t, 0)[0] == 1 && texture_column(&t, 0)[1] == 4
		&& texture_column(&t, 2)[0] == 3 && texture_column(&t, 2)[1] == 6,
		"columns are the transposed texture");
	CHECK(texture_column(&t, -4) == texture_column(&t, 0)
		&& texture_column(&t, 9) == texture_column(&t, 2), "x is clamped");
	texture_free(&t);
	blit_cases();
	if (g_fail)
		printf("pixels_test: %d check(s) failed\n", g_fail);
	else
		printf("pixels_test: ok\n");
	return (g_fail != 0);
}
