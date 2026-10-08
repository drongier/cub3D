#include "../includes/cub3d.h"

/* Ce qui ne dépend pas de la map : framebuffer */
bool	game_init(t_game *game)
{
	ft_bzero(game, sizeof(*game));
	game->fb = ft_calloc((size_t)WIDTH * HEIGHT, sizeof(uint32_t));
	return (game->fb != NULL);
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
	init_player(&game->player, &game->level, game);
	return (true);
}

void	game_unload_level(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
		texture_free(&game->textures[i++]);
	grid_free(&game->grid);
	level_free(&game->level);
}

void	game_destroy(t_game *game)
{
	game_unload_level(game);
	free(game->fb);
	game->fb = NULL;
	platform_destroy(game->platform);
	game->platform = NULL;
}
