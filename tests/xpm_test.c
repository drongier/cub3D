#include "../includes/texture.h"
#include <glob.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static int	g_fail;

#define CHECK(cond, ...) do { if (!(cond)) { g_fail++; \
	printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); \
	printf("\n"); } } while (0)

static uint32_t	pixel(const t_texture *t, int x, int y)
{
	return (((const uint32_t *)t->data)[y * t->width + x]);
}

static void	write_file(const char *path, const char *content)
{
	FILE	*f;

	f = fopen(path, "w");
	if (!f)
	{
		perror(path);
		g_fail++;
		return ;
	}
	fputs(content, f);
	fclose(f);
}

static void	test_known_pixels(void)
{
	t_texture	t;

	t.data = NULL;
	CHECK(xpm_load("textures/wolfenstein/eagle.xpm", &t), "eagle.xpm load");
	if (!g_fail)
	{
		CHECK(t.width == 64 && t.height == 64, "eagle size %dx%d", t.width, t.height);
		CHECK(t.bpp == 32 && t.size_line == 256, "eagle bpp/size_line");
		CHECK(pixel(&t, 0, 0) == 0x383838, "eagle (0,0) = %06X", pixel(&t, 0, 0));
		CHECK(pixel(&t, 10, 20) == 0x480048, "eagle (10,20) = %06X", pixel(&t, 10, 20));
		texture_free(&t);
		CHECK(t.data == NULL, "texture_free resets data");
	}
	t.data = NULL;
	CHECK(xpm_load("textures/test/north.xpm", &t), "north.xpm load");
	if (t.data)
	{
		CHECK(pixel(&t, 0, 0) == 0xFFFFFF, "north (0,0) = %06X", pixel(&t, 0, 0));
		CHECK(pixel(&t, 10, 20) == 0x303030, "north (10,20) = %06X", pixel(&t, 10, 20));
		texture_free(&t);
	}
}

static void	test_all_repo_textures(void)
{
	glob_t		g;
	t_texture	t;
	size_t		i;

	memset(&g, 0, sizeof(g));
	glob("textures/*.xpm", 0, NULL, &g);
	glob("textures/*/*.xpm", GLOB_APPEND, NULL, &g);
	CHECK(g.gl_pathc >= 51, "found only %zu xpm files", g.gl_pathc);
	i = 0;
	while (i < g.gl_pathc)
	{
		t.data = NULL;
		CHECK(xpm_load(g.gl_pathv[i], &t), "load %s", g.gl_pathv[i]);
		texture_free(&t);
		i++;
	}
	globfree(&g);
}

static void	test_short_hex_and_none(void)
{
	t_texture	t;

	t.data = NULL;
	write_file("build/tests/rgb.xpm",
		"/* XPM */\nstatic char *x[] = {\n\"2 1 2 1\",\n"
		"\"a c #F0A\",\n\"b c None\",\n\"ab\"};\n");
	CHECK(xpm_load("build/tests/rgb.xpm", &t), "rgb.xpm load");
	if (t.data)
	{
		CHECK(pixel(&t, 0, 0) == 0xFF00AA, "#F0A = %06X", pixel(&t, 0, 0));
		CHECK(pixel(&t, 1, 0) == TEX_TRANSPARENT, "None = %08X",
			pixel(&t, 1, 0));
		texture_free(&t);
	}
}

static void	expect_fail(const char *path, const char *why)
{
	t_texture	t;

	t.data = NULL;
	CHECK(!xpm_load(path, &t), "%s should fail (%s)", path, why);
	CHECK(t.data == NULL, "%s: nothing allocated on failure", path);
}

static void	test_failures(void)
{
	expect_fail("build/tests/does_not_exist.xpm", "missing file");
	expect_fail("textures", "directory");
	write_file("build/tests/truncated.xpm",
		"\"4 4 2 1\",\n\"a c #000000\",\n\"b c #FFFFFF\",\n\"abab\",\n\"baba\",\n");
	expect_fail("build/tests/truncated.xpm", "2 rows instead of 4");
	write_file("build/tests/short_row.xpm",
		"\"4 1 1 1\",\n\"a c #000000\",\n\"aaa\",\n");
	expect_fail("build/tests/short_row.xpm", "row shorter than width");
	write_file("build/tests/unknown_key.xpm",
		"\"2 1 1 1\",\n\"a c #000000\",\n\"az\",\n");
	expect_fail("build/tests/unknown_key.xpm", "pixel key not in palette");
	write_file("build/tests/named.xpm",
		"\"1 1 1 1\",\n\"a c black\",\n\"a\",\n");
	expect_fail("build/tests/named.xpm", "named color");
	write_file("build/tests/cpp3.xpm",
		"\"1 1 1 3\",\n\"abc c #000000\",\n\"abc\",\n");
	expect_fail("build/tests/cpp3.xpm", "3 chars per pixel");
	write_file("build/tests/no_read.xpm", "\"1 1 1 1\",\n\"a c #000000\",\n\"a\",\n");
	chmod("build/tests/no_read.xpm", 0);
	if (geteuid() != 0)
		expect_fail("build/tests/no_read.xpm", "no read permission");
	chmod("build/tests/no_read.xpm", 0644);
}

int	main(void)
{
	test_known_pixels();
	test_all_repo_textures();
	test_short_hex_and_none();
	test_failures();
	if (g_fail)
		printf("xpm_test: %d check(s) failed\n", g_fail);
	else
		printf("xpm_test: ok\n");
	return (g_fail != 0);
}
