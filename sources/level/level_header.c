#include "level_internal.h"
#include <stdlib.h>
#include <string.h>

/* Bits de seen : NO SO WE EA F C */
static const char	*g_ids[6] = {"NO", "SO", "WE", "EA", "F", "C"};

static int	skip_spaces(const t_line *l, int i)
{
	while (i < l->len && is_space(l->s[i]))
		i++;
	return (i);
}

static int	word_end(const t_line *l, int i)
{
	while (i < l->len && !is_space(l->s[i]))
		i++;
	return (i);
}

/* "NO ./path.xpm" : un seul mot, terminé par .xpm */
static bool	parse_texture(const t_line *l, int i, char **dst,
		t_level_error *err)
{
	int	end;

	i = skip_spaces(l, i);
	end = word_end(l, i);
	if (end == i)
		return (level_fail(err, l->num, "missing texture path"));
	if (skip_spaces(l, end) != l->len)
		return (level_fail(err, l->num, "texture path must be a single word"));
	if (end - i < 5 || strncmp(l->s + end - 4, ".xpm", 4) != 0)
		return (level_fail(err, l->num, "texture '%.*s' is not a .xpm file",
				end - i, l->s + i));
	*dst = strndup(l->s + i, end - i);
	if (!*dst)
		return (level_fail(err, l->num, "out of memory"));
	return (true);
}

/* Un canal : espaces, 1 à 3 chiffres, espaces ; renvoie -1 si invalide */
static int	parse_channel(const t_line *l, int *i)
{
	int	v;
	int	digits;

	*i = skip_spaces(l, *i);
	v = 0;
	digits = 0;
	while (*i < l->len && l->s[*i] >= '0' && l->s[*i] <= '9' && digits < 4)
	{
		v = v * 10 + (l->s[(*i)++] - '0');
		digits++;
	}
	*i = skip_spaces(l, *i);
	if (digits == 0 || digits > 3 || v > 255)
		return (-1);
	return (v);
}

/* "F 220,100,0" : exactement trois canaux de 0 à 255 */
static bool	parse_color(const t_line *l, int i, uint32_t *dst,
		t_level_error *err)
{
	int	c;
	int	v;

	*dst = 0;
	c = 0;
	while (c < 3)
	{
		v = parse_channel(l, &i);
		if (v < 0)
			return (level_fail(err, l->num,
					"color must be three numbers from 0 to 255, as R,G,B"));
		*dst = (*dst << 8) | (uint32_t)v;
		if (c < 2 && (i >= l->len || l->s[i++] != ','))
			return (level_fail(err, l->num,
					"color must be three numbers from 0 to 255, as R,G,B"));
		c++;
	}
	if (i != l->len)
		return (level_fail(err, l->num,
				"color must be three numbers from 0 to 255, as R,G,B"));
	return (true);
}

static int	find_id(const t_line *l, int start, int end)
{
	int	k;

	k = 0;
	while (k < 6)
	{
		if ((int)strlen(g_ids[k]) == end - start
			&& strncmp(l->s + start, g_ids[k], end - start) == 0)
			return (k);
		k++;
	}
	return (-1);
}

/* Indice de l'identifiant qui ouvre la ligne, -1 si aucun */
int	line_id(const t_line *l)
{
	int	start;

	start = skip_spaces(l, 0);
	return (find_id(l, start, word_end(l, start)));
}

/* Une ligne d'en-tête non vide : identifiant connu, présent une seule fois */
bool	parse_header_line(const t_line *l, t_level *lv, unsigned *seen,
		t_level_error *err)
{
	int	start;
	int	end;
	int	k;

	start = skip_spaces(l, 0);
	end = word_end(l, start);
	k = find_id(l, start, end);
	if (k < 0)
		return (level_fail(err, l->num, "unknown identifier '%.*s'",
				end - start > 20 ? 20 : end - start, l->s + start));
	if (*seen & (1u << k))
		return (level_fail(err, l->num, "%s is defined twice", g_ids[k]));
	*seen |= 1u << k;
	if (k < 4)
		return (parse_texture(l, end, &lv->tex[k], err));
	if (k == 4)
		return (parse_color(l, end, &lv->floor, err));
	return (parse_color(l, end, &lv->ceiling, err));
}

/* Que des caractères de map, dont au moins un qui n'est pas un espace */
bool	looks_like_map(const t_line *l)
{
	int	i;

	if (is_blank(l))
		return (false);
	i = 0;
	while (i < l->len)
	{
		if (!is_space(l->s[i]) && !strchr("01NSEWM", l->s[i]))
			return (false);
		i++;
	}
	return (true);
}
