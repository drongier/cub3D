#include "../includes/cub3d.h"

static t_vec2	player_cell_pos(const t_game *game)
{
	return ((t_vec2){game->player.x / BLOCK, game->player.y / BLOCK});
}

/* Mort : tout s'arrête sauf les ennemis ; un appui relance la scène */
static bool	update_dead(t_game *game, bool pressed, double dt)
{
	t_level_error	err;

	game->dead_t += dt;
	horde_update(&game->horde, &game->grid, (t_target){
		player_cell_pos(game), false, false}, dt);
	if (!pressed || game->dead_t < RESTART_DELAY)
		return (true);
	if (game_load_level(game, game->level_path, &err))
		return (true);
	level_print_error(&err);
	return (false);
}

/*
 * Scène à points d'apparition : un mutant arrive quand la vague le demande
 * et qu'un point convient ; sinon on réessaie à la frame suivante.
 */
static void	update_waves(t_game *game, double dt)
{
	t_vec2	pos;

	if (game->level.n_spawns == 0)
		return ;
	if (waves_update(&game->waves, horde_alive(&game->horde), dt))
	{
		game->hp += WAVE_HEAL;
		if (game->hp > PLAYER_MAX_HP)
			game->hp = PLAYER_MAX_HP;
		items_restock(&game->items);
	}
	if (waves_due(&game->waves)
		&& horde_pick_spawn(&game->horde, &game->grid, &game->level, &pos)
		&& horde_spawn(&game->horde, pos, game->waves.rule.breed))
		waves_spawned(&game->waves);
}

/* Trousses : retour après le délai, ramassage si le joueur est blessé */
static void	update_items(t_game *game, double dt)
{
	int	heal;

	items_update(&game->items, dt);
	heal = items_pickup(&game->items, player_cell_pos(game), game->hp,
			PLAYER_MAX_HP);
	if (heal > 0)
	{
		game->hp += heal;
		game->heal_t = HEAL_FLASH_TIME;
	}
}

static void	shoot(t_game *game)
{
	int	hit;

	hit = horde_shoot(&game->horde, &game->grid, player_cell_pos(game),
			game->player.angle);
	if (hit >= 0 && game->horde.v[hit].state == EN_DYING)
		game->kills++;
}

bool	game_update(t_game *game, double dt)
{
	bool	pressed;
	bool	fired;
	int		dmg;

	pressed = game->trigger && !game->trigger_was_down;
	game->trigger_was_down = game->trigger;
	if (game->hurt_t > 0.0)
		game->hurt_t -= dt;
	if (game->heal_t > 0.0)
		game->heal_t -= dt;
	if (game->hp <= 0)
		return (update_dead(game, pressed, dt));
	update_player(&game->player, dt);
	fired = weapon_update(&game->weapon, game->trigger,
			player_move(&game->player), dt);
	if (fired)
		shoot(game);
	dmg = horde_update(&game->horde, &game->grid, (t_target){
			player_cell_pos(game), true, fired}, dt);
	if (dmg > 0)
	{
		game->hp -= dmg;
		game->hurt_t = HURT_FLASH_TIME;
		if (game->hp <= 0)
			game->hp = 0;
	}
	if (game->hp > 0)
	{
		update_items(game, dt);
		update_waves(game, dt);
	}
	return (true);
}
