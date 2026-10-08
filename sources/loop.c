#include "../includes/cub3d.h"

static void	apply_input(t_player *player, const t_input *in)
{
	player->key_up = in->up;
	player->key_down = in->down;
	player->key_left = in->left;
	player->key_right = in->right;
	player->left_rotate = in->rot_left;
	player->right_rotate = in->rot_right;
}

void	run_loop(t_game *game)
{
	t_input	input;

	ft_bzero(&input, sizeof(input));
	while (!input.quit)
	{
		platform_poll(game->platform, &input);
		if (input.quit)
			break ;
		apply_input(&game->player, &input);
		draw_loop(game);
		platform_present(game->platform, game->fb);
	}
}
