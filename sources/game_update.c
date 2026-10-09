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

bool	game_update(t_game *game, double dt)
{
	bool	pressed;
	bool	fired;
	int		dmg;

	pressed = game->trigger && !game->trigger_was_down;
	game->trigger_was_down = game->trigger;
	if (game->hurt_t > 0.0)
		game->hurt_t -= dt;
	if (game->hp <= 0)
		return (update_dead(game, pressed, dt));
	update_player(&game->player, dt);
	fired = weapon_update(&game->weapon, game->trigger,
			player_move(&game->player), dt);
	if (fired)
		horde_shoot(&game->horde, &game->grid, player_cell_pos(game),
			game->player.angle);
	dmg = horde_update(&game->horde, &game->grid, (t_target){
			player_cell_pos(game), true, fired}, dt);
	if (dmg > 0)
	{
		game->hp -= dmg;
		game->hurt_t = HURT_FLASH_TIME;
		if (game->hp <= 0)
			game->hp = 0;
	}
	return (true);
}
