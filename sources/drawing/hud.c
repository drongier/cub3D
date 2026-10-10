#include "../../includes/cub3d.h"

/* Texte blanc avec une ombre noire décalée d'un pixel agrandi */
static void	text(t_game *g, t_text t, const char *s)
{
	t_text	shadow;

	shadow = t;
	shadow.x += t.scale;
	shadow.y += t.scale;
	shadow.color = 0x000000;
	font_draw(g->fb, WIDTH, HEIGHT, shadow, s);
	font_draw(g->fb, WIDTH, HEIGHT, t, s);
}

static void	centered(t_game *g, int y, int scale, const char *s)
{
	text(g, (t_text){(WIDTH - font_width(s, scale)) / 2, y, scale, 0xFFFFFF},
		s);
}

static void	tint(t_game *g, uint32_t color, int alpha)
{
	t_canvas	c;
	int			y;

	c = (t_canvas){g->fb, WIDTH, 0, 0, WIDTH, HEIGHT};
	y = -1;
	while (++y < HEIGHT)
		fill_span(&c, y, 0, WIDTH, color, alpha);
}

/* Barre de vie en bas à gauche, du vert au rouge */
static void	health(t_game *g)
{
	t_canvas	c;
	char		buf[16];
	uint32_t	color;
	int			y;

	c = (t_canvas){g->fb, WIDTH, 0, 0, WIDTH, HEIGHT};
	text(g, (t_text){16, HEIGHT - 40, 3, 0xFFFFFF}, "VIE");
	color = 0x30C040;
	if (g->hp <= 60)
		color = 0xE0C020;
	if (g->hp <= 30)
		color = 0xE03020;
	y = HEIGHT - 42;
	while (++y < HEIGHT - 16)
	{
		fill_span(&c, y, 80, 284, 0x000000, 160);
		fill_span(&c, y, 82, 82 + 200 * g->hp / PLAYER_MAX_HP, color, 256);
	}
	snprintf(buf, sizeof(buf), "%d", g->hp);
	text(g, (t_text){296, HEIGHT - 40, 3, 0xFFFFFF}, buf);
}

/* Vagues : numéro et vivants en haut, compte à rebours pendant la pause */
static void	waves(t_game *g)
{
	char	buf[48];
	double	left;

	snprintf(buf, sizeof(buf), "VAGUE %d   MUTANTS %d", g->waves.index,
		horde_alive(&g->horde));
	text(g, (t_text){16, 16, 3, 0xFFFFFF}, buf);
	left = waves_countdown(&g->waves);
	if (g->waves.state != WAVE_PAUSE || g->hp <= 0)
		return ;
	if (g->waves.index > 1)
	{
		snprintf(buf, sizeof(buf), "VAGUE %d NETTOYEE  +%d VIE",
			g->waves.index - 1, WAVE_HEAL);
		text(g, (t_text){(WIDTH - font_width(buf, 3)) / 2, 150, 3,
			0x80FF80}, buf);
	}
	snprintf(buf, sizeof(buf), "VAGUE %d", g->waves.index);
	centered(g, 220, 8, buf);
	snprintf(buf, sizeof(buf), "%d", (int)ceil(left));
	centered(g, 300, 6, buf);
}

static void	counter(t_game *g)
{
	char	buf[32];
	int		alive;

	if (g->level.n_spawns > 0)
	{
		waves(g);
		return ;
	}
	if (g->horde.n == 0)
		return ;
	alive = horde_alive(&g->horde);
	snprintf(buf, sizeof(buf), "MUTANTS %d/%d", alive, g->horde.n);
	text(g, (t_text){16, 16, 3, 0xFFFFFF}, buf);
	if (alive == 0)
		text(g, (t_text){16, 46, 2, 0x80FF80}, "NIVEAU NETTOYE !");
}

/* Score de la partie en vagues : vagues tenues, mutants abattus */
static void	score(t_game *g)
{
	char	buf[64];

	snprintf(buf, sizeof(buf), "VAGUES TENUES %d   MUTANTS ABATTUS %d",
		g->waves.index - 1, g->kills);
	centered(g, 360, 3, buf);
}

void	draw_hud(t_game *g)
{
	if (g->hurt_t > 0.0 && g->hp > 0)
		tint(g, 0xC00000, (int)(110 * g->hurt_t / HURT_FLASH_TIME));
	if (g->hp <= 0)
	{
		tint(g, 0x800000, 60 + (int)(100 * fmin(g->dead_t, 1.0)));
		centered(g, 260, 8, "VOUS ETES MORT");
		if (g->level.n_spawns > 0)
			score(g);
		if (g->dead_t >= RESTART_DELAY)
			centered(g, 420, 3, "ESPACE OU CLIC POUR RECOMMENCER");
	}
	health(g);
	counter(g);
}
