#include "../includes/level.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int	g_fail;

#define CHECK(cond, ...) do { if (!(cond)) { g_fail++; \
	printf("FAIL %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); \
	printf("\n"); } } while (0)

#define HEAD "NO a.xpm\nSO b.xpm\nWE c.xpm\nEA d.xpm\nF 220,100,0\nC 1,2,3\n"
#define MAP "\n1111111\n1000001\n100N001\n1000001\n1111111\n"

static bool	parse(const char *text, t_level *lv, t_level_error *err)
{
	return (level_parse(text, strlen(text), lv, err));
}

/* Doit être refusé, avec un message qui contient want, à la ligne line */
static void	expect_error(const char *text, int line, const char *want)
{
	t_level			lv;
	t_level_error	err;

	if (parse(text, &lv, &err))
	{
		g_fail++;
		printf("FAIL accepted, expected '%s':\n%s\n", want, text);
		level_free(&lv);
		return ;
	}
	if (!strstr(err.msg, want) || err.line != line)
	{
		g_fail++;
		printf("FAIL expected line %d '%s', got line %d '%s'\n", line, want,
			err.line, err.msg);
	}
	if (lv.cells || lv.tex[0])
	{
		g_fail++;
		printf("FAIL out not empty after an error ('%s')\n", want);
	}
}

static void	valid_cases(void)
{
	t_level			lv;
	t_level_error	err;

	CHECK(parse(HEAD MAP, &lv, &err), "minimal map: %s", err.msg);
	CHECK(lv.w == 7 && lv.h == 5 && lv.spawn_x == 3 && lv.spawn_y == 2
		&& lv.spawn_dir == 'N' && lv.floor == 0xDC6400 && lv.ceiling == 0x010203
		&& strcmp(lv.tex[TEX_NO], "a.xpm") == 0
		&& strcmp(lv.tex[TEX_EA], "d.xpm") == 0
		&& memcmp(lv.cells + 14, "1000001", 7) == 0,
		"minimal map content");
	level_free(&lv);
	CHECK(parse("  C 1, 2 ,3\n\tEA d.xpm\nF 0,0,0\n\nWE c.xpm\nSO b.xpm\n"
			"NO a.xpm\r\n" MAP, &lv, &err),
		"any order, spaces, tabs, CRLF: %s", err.msg);
	level_free(&lv);
	CHECK(parse(HEAD "\n  111\n111011\n10S01\n111111", &lv, &err),
		"indented map, no final newline: %s", err.msg);
	CHECK(lv.w == 6 && lv.h == 4 && lv.cells[0] == ' ' && lv.cells[2] == '1'
		&& lv.cells[3 * 6 + 5] == '1', "outside stays ' ', last char kept");
	level_free(&lv);
}

static void	big_map(int side, bool want_ok)
{
	char			*text;
	char			*p;
	t_level			lv;
	t_level_error	err;
	int				y;

	text = malloc(strlen(HEAD) + (size_t)(side + 1) * side + 2);
	p = text + sprintf(text, "%s", HEAD);
	y = -1;
	while (++y < side)
	{
		memset(p, (y == 0 || y == side - 1) ? '1' : '0', side);
		p[0] = '1';
		p[side - 1] = '1';
		if (y == side / 2)
			p[side / 2] = 'E';
		p[side] = '\n';
		p += side + 1;
	}
	*p = '\0';
	CHECK(parse(text, &lv, &err) == want_ok, "%dx%d open room: %s", side,
		side, err.msg);
	if (want_ok)
		level_free(&lv);
	free(text);
}

int	main(void)
{
	valid_cases();
	big_map(LEVEL_MAX_SIDE, true);
	big_map(LEVEL_MAX_SIDE + 1, false);
	expect_error("NO a.xpm\nNO b.xpm\n" MAP, 2, "NO is defined twice");
	expect_error("N a.xpm\n" MAP, 1, "unknown identifier 'N'");
	expect_error("NO a.xpm\n" MAP, 3, "the map starts before SO, WE, EA, F, C");
	expect_error("NOO a.xpm\n" MAP, 1, "unknown identifier 'NOO'");
	expect_error("SO\n" HEAD MAP, 1, "missing texture path");
	expect_error("NO a.png\n" MAP, 1, "is not a .xpm file");
	expect_error("NO a b.xpm\n" MAP, 1, "single word");
	expect_error("NO a.xpm\nSO b.xpm\nWE c.xpm\nEA d.xpm\nF a,b,c\n", 5,
		"three numbers");
	expect_error("NO a.xpm\nSO b.xpm\nWE c.xpm\nEA d.xpm\nF 12x,4,5\n", 5,
		"three numbers");
	expect_error("NO a.xpm\nSO b.xpm\nWE c.xpm\nEA d.xpm\nF 1,2,3,\n", 5,
		"three numbers");
	expect_error("NO a.xpm\nSO b.xpm\nWE c.xpm\nEA d.xpm\nF 1,2\n", 5,
		"three numbers");
	expect_error("NO a.xpm\nSO b.xpm\nWE c.xpm\nEA d.xpm\nF 1,2,256\n", 5,
		"three numbers");
	expect_error("NO a.xpm\nSO b.xpm\nWE c.xpm\nEA d.xpm\nF 1,2,0003\n", 5,
		"three numbers");
	expect_error("NO a.xpm\nhello world\n", 2, "unknown identifier 'hello'");
	expect_error(HEAD, 0, "no map");
	expect_error("NO a.xpm\n", 0, "missing SO, WE, EA, F, C");
	expect_error(HEAD "\n1111\n1N01\n\n1111\n", 11, "content after the map");
	expect_error(HEAD MAP "NO a.xpm\n", 13, "identifier after the map");
	expect_error(HEAD "\n1111\n1NS1\n1111\n", 9, "more than one starting");
	expect_error(HEAD "\n1111\n1001\n1111\n", 8, "no starting position");
	expect_error(HEAD "\n1111\n1N21\n1111\n", 9, "unexpected character '2'");
	expect_error(HEAD "\n1111\n1N01\n1101\n", 9, "not closed");
	expect_error(HEAD "\n1111\nN001\n1111\n", 9, "not closed");
	expect_error(HEAD "\n 111\n1N01\n1111\n", 9, "not closed");
	{
		t_level			lv;
		t_level_error	err;

		CHECK(!level_parse("NO a.xpm\0\n", 10, &lv, &err)
			&& strstr(err.msg, "NUL"), "NUL byte refused");
	}
	if (g_fail)
		printf("level_test: %d check(s) failed\n", g_fail);
	else
		printf("level_test: ok\n");
	return (g_fail != 0);
}
