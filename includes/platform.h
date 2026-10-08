#ifndef PLATFORM_H
# define PLATFORM_H

# include <stdbool.h>
# include <stdint.h>

/* État courant des commandes, mis à jour par platform_poll */
typedef struct s_input
{
	bool	up;
	bool	down;
	bool	left;
	bool	right;
	bool	rot_left;
	bool	rot_right;
	bool	quit;
}	t_input;

typedef struct s_platform	t_platform;

t_platform	*platform_init(int width, int height, bool vsync);
void		platform_poll(t_platform *p, t_input *in);
void		platform_present(t_platform *p, const uint32_t *fb);
void		platform_set_title(t_platform *p, const char *title);
uint64_t	platform_ticks(void);
double		platform_ticks_to_ms(uint64_t ticks);
void		platform_sleep_ns(uint64_t ns);
void		platform_destroy(t_platform *p);

#endif
