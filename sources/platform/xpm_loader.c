#include "../../includes/texture.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#define XPM_MAX_SIDE 16384

typedef struct s_xpm
{
	char	**str;
	int		count;
	int		width;
	int		height;
	int		ncolors;
	int		cpp;
	int32_t	*lut;
}	t_xpm;

static char	*read_file(const char *path)
{
	int		fd;
	char	*buf;
	char	*tmp;
	size_t	len;
	size_t	cap;
	ssize_t	r;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	cap = 16384;
	len = 0;
	buf = malloc(cap + 1);
	while (buf)
	{
		if (len == cap)
		{
			cap *= 2;
			tmp = realloc(buf, cap + 1);
			if (!tmp)
				free(buf);
			buf = tmp;
			if (!buf)
				break ;
		}
		r = read(fd, buf + len, cap - len);
		if (r <= 0)
		{
			if (r < 0)
			{
				free(buf);
				buf = NULL;
			}
			break ;
		}
		len += (size_t)r;
	}
	close(fd);
	if (buf)
		buf[len] = '\0';
	return (buf);
}

/* Garde les chaînes entre guillemets (terminées par '\0' sur place),
   saute les commentaires C. */
static bool	collect_strings(char *p, t_xpm *x)
{
	int		cap;
	char	**tmp;
	char	*end;

	cap = 64;
	x->str = malloc(cap * sizeof(char *));
	while (x->str && *p)
	{
		if (p[0] == '/' && p[1] == '*')
		{
			p = strstr(p + 2, "*/");
			if (!p)
				return (false);
			p += 2;
		}
		else if (*p == '"')
		{
			end = strchr(p + 1, '"');
			if (!end)
				return (false);
			if (x->count == cap)
			{
				cap *= 2;
				tmp = realloc(x->str, cap * sizeof(char *));
				if (!tmp)
					return (false);
				x->str = tmp;
			}
			*end = '\0';
			x->str[x->count++] = p + 1;
			p = end + 1;
		}
		else
			p++;
	}
	return (x->str != NULL);
}

static bool	parse_header(t_xpm *x)
{
	if (x->count < 1 || sscanf(x->str[0], "%d %d %d %d",
			&x->width, &x->height, &x->ncolors, &x->cpp) != 4)
		return (false);
	if (x->width <= 0 || x->height <= 0 || x->width > XPM_MAX_SIDE
		|| x->height > XPM_MAX_SIDE || (x->cpp != 1 && x->cpp != 2))
		return (false);
	if (x->ncolors <= 0 || x->ncolors > (x->cpp == 1 ? 256 : 65536))
		return (false);
	return (x->count >= 1 + x->ncolors + x->height);
}

static int	key_of(const char *s, int cpp)
{
	if (cpp == 1)
		return ((unsigned char)s[0]);
	return (((unsigned char)s[0] << 8) | (unsigned char)s[1]);
}

static int	hex_value(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

/* "#RRGGBB", "#RGB" ou "None" (TEX_TRANSPARENT) */
static bool	parse_color_value(const char *v, size_t len, int32_t *out)
{
	int32_t	value;
	size_t	i;
	int		h;

	if (len == 4 && strncasecmp(v, "None", 4) == 0)
	{
		*out = (int32_t)TEX_TRANSPARENT;
		return (true);
	}
	if (v[0] != '#' || (len != 7 && len != 4))
		return (false);
	value = 0;
	i = 1;
	while (i < len)
	{
		h = hex_value(v[i]);
		if (h < 0)
			return (false);
		if (len == 4)
			value = (value << 8) | (h * 17);
		else
			value = (value << 4) | h;
		i++;
	}
	*out = value;
	return (true);
}

static bool	next_token(const char **p, const char **tok, size_t *len)
{
	while (**p == ' ' || **p == '\t')
		(*p)++;
	if (!**p)
		return (false);
	*tok = *p;
	while (**p && **p != ' ' && **p != '\t')
		(*p)++;
	*len = (size_t)(*p - *tok);
	return (true);
}

/* "<clé> [m v] [s v] c <couleur>" : seule la valeur du type "c" compte */
static bool	parse_color_line(const char *s, t_xpm *x)
{
	const char	*p;
	const char	*tok;
	size_t		len;
	bool		want_value;
	int32_t		color;

	if (strlen(s) < (size_t)x->cpp)
		return (false);
	p = s + x->cpp;
	want_value = false;
	while (next_token(&p, &tok, &len))
	{
		if (want_value)
		{
			if (!parse_color_value(tok, len, &color))
				return (false);
			x->lut[key_of(s, x->cpp)] = color;
			return (true);
		}
		want_value = (len == 1 && tok[0] == 'c');
	}
	return (false);
}

static bool	build_lut(t_xpm *x)
{
	size_t	size;
	int		i;

	size = (x->cpp == 1) ? 256 : 65536;
	x->lut = malloc(size * sizeof(int32_t));
	if (!x->lut)
		return (false);
	memset(x->lut, 0xFF, size * sizeof(int32_t));
	i = 0;
	while (i < x->ncolors)
	{
		if (!parse_color_line(x->str[1 + i], x))
			return (false);
		i++;
	}
	return (true);
}

static bool	fill_pixels(t_xpm *x, uint32_t *px)
{
	const char	*row;
	int32_t		color;
	int			y;
	int			i;

	y = 0;
	while (y < x->height)
	{
		row = x->str[1 + x->ncolors + y];
		if (strlen(row) < (size_t)x->width * x->cpp)
			return (false);
		i = 0;
		while (i < x->width)
		{
			color = x->lut[key_of(row + i * x->cpp, x->cpp)];
			if (color < 0)
				return (false);
			px[y * x->width + i] = (uint32_t)color;
			i++;
		}
		y++;
	}
	return (true);
}

bool	xpm_load(const char *path, t_texture *out)
{
	t_xpm		x;
	char		*buf;
	uint32_t	*px;
	bool		ok;

	memset(&x, 0, sizeof(x));
	px = NULL;
	buf = read_file(path);
	ok = buf && collect_strings(buf, &x) && parse_header(&x) && build_lut(&x);
	if (ok)
	{
		px = malloc((size_t)x.width * x.height * sizeof(uint32_t));
		ok = px && fill_pixels(&x, px);
	}
	free(x.lut);
	free(x.str);
	free(buf);
	if (!ok)
	{
		free(px);
		return (false);
	}
	out->data = (char *)px;
	out->columns = NULL;
	out->width = x.width;
	out->height = x.height;
	out->bpp = 32;
	out->size_line = x.width * (int)sizeof(uint32_t);
	return (true);
}

void	texture_free(t_texture *t)
{
	free(t->data);
	t->data = NULL;
	free(t->columns);
	t->columns = NULL;
}
