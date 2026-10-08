#include "level_internal.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void	level_free(t_level *lv)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		free(lv->tex[i]);
		lv->tex[i++] = NULL;
	}
	free(lv->cells);
	lv->cells = NULL;
}

static const char	*missing_ids(unsigned seen)
{
	static const char	*names[6] = {"NO", "SO", "WE", "EA", "F", "C"};
	static char			buf[32];
	int					k;

	buf[0] = '\0';
	k = 0;
	while (k < 6)
	{
		if (!(seen & (1u << k)))
		{
			if (buf[0])
				strcat(buf, ", ");
			strcat(buf, names[k]);
		}
		k++;
	}
	return (buf);
}

/*
 * En-tête dans n'importe quel ordre, puis la map, qui finit le fichier. Une
 * ligne qui ressemble à la map et ne commence pas par un identifiant ouvre
 * la map ; une fois les six identifiants lus, toute ligne l'ouvre.
 */
static bool	parse_lines(const t_lines *lines, t_level *lv, t_level_error *err)
{
	unsigned	seen;
	const t_line	*l;
	int			i;

	seen = 0;
	i = -1;
	while (++i < lines->n)
	{
		l = &lines->v[i];
		if (is_blank(l))
			continue ;
		if (looks_like_map(l) && (seen == SEEN_ALL || line_id(l) < 0))
		{
			if (seen != SEEN_ALL)
				return (level_fail(err, l->num, "the map starts before %s",
						missing_ids(seen)));
			return (parse_map(lines, i, lv, err));
		}
		if (!parse_header_line(l, lv, &seen, err))
			return (false);
	}
	if (seen != SEEN_ALL)
		return (level_fail(err, 0, "missing %s", missing_ids(seen)));
	return (level_fail(err, 0, "no map after the identifiers"));
}

bool	level_parse(const char *text, size_t len, t_level *out,
		t_level_error *err)
{
	t_lines	lines;
	bool	ok;

	memset(out, 0, sizeof(*out));
	err->line = 0;
	err->msg[0] = '\0';
	if (memchr(text, '\0', len))
		return (level_fail(err, 0, "the file contains a NUL byte"));
	if (!lines_split(text, len, &lines, err))
		return (false);
	ok = parse_lines(&lines, out, err);
	free(lines.v);
	if (!ok)
		level_free(out);
	return (ok);
}

static bool	read_all(int fd, char **buf, size_t *len, t_level_error *err)
{
	struct stat	st;
	ssize_t		r;

	if (fstat(fd, &st) < 0 || !S_ISREG(st.st_mode))
		return (level_fail(err, 0, "not a regular file"));
	if (st.st_size > LEVEL_MAX_FILE)
		return (level_fail(err, 0, "the file is larger than %d bytes",
				LEVEL_MAX_FILE));
	*buf = malloc((size_t)st.st_size + 1);
	if (!*buf)
		return (level_fail(err, 0, "out of memory"));
	*len = 0;
	r = 1;
	while (r > 0 && *len < (size_t)st.st_size)
	{
		r = read(fd, *buf + *len, (size_t)st.st_size - *len);
		if (r > 0)
			*len += (size_t)r;
	}
	if (r < 0)
	{
		free(*buf);
		return (level_fail(err, 0, "read error: %s", strerror(errno)));
	}
	return (true);
}

bool	level_load(const char *path, t_level *out, t_level_error *err)
{
	size_t	n;
	int		fd;
	char	*buf;
	size_t	len;
	bool	ok;

	memset(out, 0, sizeof(*out));
	n = strlen(path);
	if (n < 5 || strcmp(path + n - 4, ".cub") != 0 || path[n - 5] == '/')
		return (level_fail(err, 0, "'%s' is not a .cub file", path));
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (level_fail(err, 0, "cannot open '%s': %s", path,
				strerror(errno)));
	ok = read_all(fd, &buf, &len, err);
	close(fd);
	if (!ok)
		return (false);
	ok = level_parse(buf, len, out, err);
	free(buf);
	return (ok);
}

void	level_print_error(const t_level_error *err)
{
	if (err->line > 0)
		printf("Error\nline %d: %s\n", err->line, err->msg);
	else
		printf("Error\n%s\n", err->msg);
}
