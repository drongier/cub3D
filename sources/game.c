#include "../includes/cub3d.h"

/* Ce qui ne dépend pas de la map : framebuffer, arme */
bool	game_init(t_game *game, t_level_error *err)
{
	char	path[256];
	int		i;

	ft_bzero(game, sizeof(*game));
	err->line = 0;
	weapon_init(&game->weapon);
	game->fb = ft_calloc((size_t)WIDTH * HEIGHT, sizeof(uint32_t));
	if (!game->fb)
		return (snprintf(err->msg, sizeof(err->msg), "out of memory"), false);
	i = -1;
	while (++i < WEAPON_FRAMES)
	{
		snprintf(path, sizeof(path), "%s_%d.xpm", WEAPON_XPM_PREFIX, i);
		if (!xpm_load(path, &game->weapon_tex[i]))
			return (snprintf(err->msg, sizeof(err->msg),
					"cannot load texture '%s'", path), false);
	}
	if (!xpm_load(MUTANT_XPM, &game->mutant_tex)
		|| !texture_build_columns(&game->mutant_tex))
		return (snprintf(err->msg, sizeof(err->msg),
				"cannot load texture '%s'", MUTANT_XPM), false);
	return (true);
}

static void	init_player(t_player *player, const t_level *lv, t_game *game)
{
	ft_bzero(player, sizeof(*player));
	player->game = game;
	player->x = lv->spawn_x * BLOCK + BLOCK / 2.0f;
	player->y = lv->spawn_y * BLOCK + BLOCK / 2.0f;
	if (lv->spawn_dir == 'N')
		player->angle = 3 * PI / 2;
	else if (lv->spawn_dir == 'S')
		player->angle = PI / 2;
	else if (lv->spawn_dir == 'W')
		player->angle = PI;
	else
		player->angle = 0;
}

static bool	load_textures(t_game *game, t_level_error *err)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (!xpm_load(game->level.tex[i], &game->textures[i]))
		{
			err->line = 0;
			snprintf(err->msg, sizeof(err->msg),
				"cannot load texture '%s'", game->level.tex[i]);
			return (false);
		}
		if (!texture_build_columns(&game->textures[i]))
		{
			err->line = 0;
			snprintf(err->msg, sizeof(err->msg), "out of memory");
			return (false);
		}
		i++;
	}
	return (true);
}

/*
 * Charge une scène : fichier, textures, grille, joueur. En cas d'échec, err
 * est rempli et le jeu revient à l'état sans map ; la fenêtre n'est pas
 * touchée, on peut donc changer de map sans la fermer.
 */
bool	game_load_level(t_game *game, const char *path, t_level_error *err)
{
	game_unload_level(game);
	if (!level_load(path, &game->level, err))
		return (false);
	if (!load_textures(game, err))
	{
		game_unload_level(game);
		return (false);
	}
	if (!grid_init(&game->grid, game->level.cells, game->level.w,
			game->level.h))
	{
		err->line = 0;
		snprintf(err->msg, sizeof(err->msg), "out of memory");
		game_unload_level(game);
		return (false);
	}
	if (!horde_init(&game->horde, &game->level))
	{
		err->line = 0;
		snprintf(err->msg, sizeof(err->msg), "out of memory");
		game_unload_level(game);
		return (false);
	}
	init_player(&game->player, &game->level, game);
	weapon_init(&game->weapon);
	game->level_path = path;
	game->hp = PLAYER_MAX_HP;
	game->hurt_t = 0.0;
	game->dead_t = 0.0;
	return (true);
}

void	game_unload_level(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
		texture_free(&game->textures[i++]);
	grid_free(&game->grid);
	horde_free(&game->horde);
	level_free(&game->level);
}

void	game_destroy(t_game *game)
{
	int	i;

	game_unload_level(game);
	i = 0;
	while (i < WEAPON_FRAMES)
		texture_free(&game->weapon_tex[i++]);
	texture_free(&game->mutant_tex);
	free(game->fb);
	game->fb = NULL;
	platform_destroy(game->platform);
	game->platform = NULL;
}
