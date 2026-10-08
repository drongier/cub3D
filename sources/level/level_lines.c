#include "level_internal.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

bool	level_fail(t_level_error *err, int line, const char *fmt, ...)
{
	va_list	ap;

	err->line = line;
	va_start(ap, fmt);
	vsnprintf(err->msg, sizeof(err->msg), fmt, ap);
	va_end(ap);
	return (false);
}

bool	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r');
}

bool	is_blank(const t_line *l)
{
	int	i;

	i = 0;
	while (i < l->len && is_space(l->s[i]))
		i++;
	return (i == l->len);
}

static int	count_lines(const char *text, size_t len)
{
	size_t	i;
	int		n;

	n = 1;
	i = 0;
	while (i < len)
		if (text[i++] == '\n')
			n++;
	return (n);
}

/* Découpe sans copier ; une ligne vide finale après le dernier '\n' compte */
bool	lines_split(const char *text, size_t len, t_lines *out,
		t_level_error *err)
{
	size_t	start;
	size_t	i;

	out->v = malloc(sizeof(t_line) * count_lines(text, len));
	if (!out->v)
		return (level_fail(err, 0, "out of memory"));
	out->n = 0;
	start = 0;
	i = 0;
	while (i <= len)
	{
		if (i == len || text[i] == '\n')
		{
			out->v[out->n].s = text + start;
			out->v[out->n].len = (int)(i - start);
			if (out->v[out->n].len > 0 && text[i - 1] == '\r')
				out->v[out->n].len--;
			out->v[out->n].num = out->n + 1;
			out->n++;
			start = i + 1;
		}
		i++;
	}
	return (true);
}
