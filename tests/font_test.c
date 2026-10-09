#include "../includes/font.h"
#include <stdio.h>
#include <string.h>

static int	g_fail;

#define CHECK(cond, ...) do { if (!(cond)) { g_fail++; \
	printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); \
	printf("\n"); } } while (0)

#define W 40
#define H 20

int	main(void)
{
	uint32_t	fb[W * H];
	int			lit;

	CHECK(font_width("", 2) == 0 && font_width("I", 1) == 5
		&& font_width("AB", 2) == 22, "widths");
	memset(fb, 0, sizeof(fb));
	font_draw(fb, W, H, (t_text){0, 0, 1, 7}, "I");
	CHECK(fb[0] == 0 && fb[1] == 7 && fb[2] == 7 && fb[3] == 7 && fb[4] == 0
		&& fb[1 * W + 2] == 7 && fb[1 * W + 1] == 0, "the I glyph");
	memset(fb, 0, sizeof(fb));
	font_draw(fb, W, H, (t_text){-3, 10, 2, 7}, "-?.");
	lit = 0;
	for (int i = 0; i < W * H; i++)
		lit += fb[i] == 7;
	CHECK(lit == 7 * 2,
		"dash clipped on the left, unknown char blank, dot below: %d", lit);
	if (g_fail)
		printf("font_test: %d check(s) failed\n", g_fail);
	else
		printf("font_test: ok\n");
	return (g_fail != 0);
}
