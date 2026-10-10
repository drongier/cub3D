#ifndef WAVES_H
# define WAVES_H

# include <stdbool.h>
# include "enemy.h"

/* Pause avant la première vague, puis entre deux vagues, en secondes */
# define WAVE_FIRST_DELAY 3.0
# define WAVE_BREAK 6.0
/* Vie rendue au joueur à la fin de chaque vague */
# define WAVE_HEAL 25
# define WAVE_MAX_COUNT 40

/*
 * PAUSE : compte à rebours avant la vague index. SPAWN : les mutants
 * arrivent un par un. FIGHT : tous sont arrivés ; la vague est finie quand
 * plus aucun mutant n'est vivant.
 */
enum e_wave_state
{
	WAVE_PAUSE,
	WAVE_SPAWN,
	WAVE_FIGHT
};

typedef struct s_wave_rule
{
	int		count;
	double	interval;
	t_breed	breed;
}	t_wave_rule;

/* t : temps passé en pause, ou depuis la dernière apparition */
typedef struct s_waves
{
	int			index;
	int			state;
	double		t;
	double		pause;
	int			spawned;
	t_wave_rule	rule;
}	t_waves;

/* Vague 1, 2, ... : plus nombreuse, plus rapprochée, plus coriace */
t_wave_rule	wave_rule(int index);
void		waves_start(t_waves *w);
/* Avance de dt ; true quand la vague en cours vient d'être nettoyée */
bool		waves_update(t_waves *w, int alive, double dt);
/* Un mutant doit-il apparaître maintenant ? Puis waves_spawned s'il l'a pu */
bool		waves_due(const t_waves *w);
void		waves_spawned(t_waves *w);
/* Secondes avant la prochaine vague, 0 hors pause */
double		waves_countdown(const t_waves *w);

#endif
