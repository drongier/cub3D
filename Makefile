NAME		?= cub3D
BUILD		?= build
OPT			?= -O2

LIBFT		:= libft/libft.a
SDL_CFLAGS	:= $(shell pkg-config --cflags sdl3 2>/dev/null)
SDL_LIBS	:= $(shell pkg-config --libs sdl3 2>/dev/null)

CFLAGS		:= -Wall -Wextra -Werror -MMD -MP $(OPT) -g $(EXTRA_CFLAGS) $(SDL_CFLAGS)
LDFLAGS		:= $(EXTRA_LDFLAGS)
LDLIBS		:= $(LIBFT) $(SDL_LIBS) -lm

ENEMY_SRC	:= sources/enemy/enemy.c sources/enemy/enemy_move.c \
			   sources/enemy/enemy_view.c sources/enemy/enemy_spawn.c

LEVEL_SRC	:= sources/level/level.c sources/level/level_lines.c \
			   sources/level/level_header.c sources/level/level_map.c

SRC			:= sources/main.c sources/game.c sources/game_update.c \
			   sources/loop.c sources/options.c sources/stats.c sources/motion.c \
			   sources/raster.c sources/grid.c sources/raycast.c sources/pixels.c \
			   sources/font.c \
			   sources/weapon.c sources/waves.c \
			   $(LEVEL_SRC) $(ENEMY_SRC) \
			   sources/platform/platform_sdl.c \
			   sources/platform/xpm_loader.c \
			   sources/drawing/minimap.c sources/drawing/sprites.c \
			   sources/drawing/hud.c \
			   sources/drawing/drawing.c \
			   sources/drawing/player.c \
			   sources/drawing/utils.c
OBJ			:= $(SRC:%.c=$(BUILD)/%.o)

TEST_BINS	:= $(BUILD)/tests/xpm_test $(BUILD)/tests/options_test \
			   $(BUILD)/tests/stats_test $(BUILD)/tests/motion_test \
			   $(BUILD)/tests/raster_test $(BUILD)/tests/raycast_test \
			   $(BUILD)/tests/pixels_test $(BUILD)/tests/level_test \
			   $(BUILD)/tests/weapon_test $(BUILD)/tests/enemy_test \
			   $(BUILD)/tests/font_test $(BUILD)/tests/waves_test

ifeq ($(filter clean fclean,$(MAKECMDGOALS)),)
ifeq ($(SDL_LIBS),)
$(error SDL3 introuvable via pkg-config. macOS : brew install sdl3 | Linux récent : paquet libsdl3-dev | sinon : compiler SDL3 depuis https://github.com/libsdl-org/SDL)
endif
endif

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C libft

debug:
	$(MAKE) NAME=cub3D_debug BUILD=build-debug OPT=-O0 \
		EXTRA_CFLAGS="-fsanitize=address,undefined" \
		EXTRA_LDFLAGS="-fsanitize=address,undefined"

test: $(TEST_BINS)
	@for t in $(TEST_BINS); do ./$$t || exit 1; done

$(BUILD)/tests/xpm_test: tests/xpm_test.c $(BUILD)/sources/platform/xpm_loader.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(filter %.c %.o,$^) -o $@

$(BUILD)/tests/options_test: tests/options_test.c $(BUILD)/sources/options.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(filter %.c %.o,$^) -o $@

$(BUILD)/tests/stats_test: tests/stats_test.c $(BUILD)/sources/stats.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(filter %.c %.o,$^) -lm -o $@

$(BUILD)/tests/motion_test: tests/motion_test.c $(BUILD)/sources/motion.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(filter %.c %.o,$^) -lm -o $@

$(BUILD)/tests/raster_test: tests/raster_test.c $(BUILD)/sources/raster.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(filter %.c %.o,$^) -lm -o $@

$(BUILD)/tests/raycast_test: tests/raycast_test.c $(BUILD)/sources/raycast.o \
		$(BUILD)/sources/grid.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(filter %.c %.o,$^) -lm -o $@

$(BUILD)/tests/pixels_test: tests/pixels_test.c $(BUILD)/sources/pixels.o \
		$(BUILD)/sources/platform/xpm_loader.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(filter %.c %.o,$^) -o $@

$(BUILD)/tests/level_test: tests/level_test.c $(LEVEL_SRC:%.c=$(BUILD)/%.o)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(filter %.c %.o,$^) -o $@

$(BUILD)/tests/weapon_test: tests/weapon_test.c $(BUILD)/sources/weapon.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(filter %.c %.o,$^) -lm -o $@

$(BUILD)/tests/enemy_test: tests/enemy_test.c $(ENEMY_SRC:%.c=$(BUILD)/%.o) \
		$(BUILD)/sources/grid.o $(BUILD)/sources/raycast.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(filter %.c %.o,$^) -lm -o $@

$(BUILD)/tests/font_test: tests/font_test.c $(BUILD)/sources/font.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(filter %.c %.o,$^) -o $@

$(BUILD)/tests/waves_test: tests/waves_test.c $(BUILD)/sources/waves.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(filter %.c %.o,$^) -lm -o $@

clean:
	rm -rf build build-debug
	$(MAKE) -C libft clean

fclean: clean
	rm -f cub3D cub3D_debug
	$(MAKE) -C libft fclean

re: fclean
	$(MAKE) all

.PHONY: all debug test clean fclean re

-include $(OBJ:.o=.d) $(wildcard $(BUILD)/tests/*.d)
