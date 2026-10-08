#ifndef LEVEL_INTERNAL_H
# define LEVEL_INTERNAL_H

# include "../../includes/level.h"

/* Une ligne du fichier, sans le '\n' ni un '\r' final */
typedef struct s_line
{
	const char	*s;
	int			len;
	int			num;
}	t_line;

typedef struct s_lines
{
	t_line	*v;
	int		n;
}	t_lines;

bool	level_fail(t_level_error *err, int line, const char *fmt, ...);
bool	lines_split(const char *text, size_t len, t_lines *out,
			t_level_error *err);
bool	is_blank(const t_line *l);
bool	is_space(char c);
bool	parse_header_line(const t_line *l, t_level *lv, unsigned *seen,
			t_level_error *err);
bool	looks_like_map(const t_line *l);
int		line_id(const t_line *l);
bool	parse_map(const t_lines *lines, int first, t_level *lv,
			t_level_error *err);

# define SEEN_ALL 0x3F

#endif
