NAME		?= cub3D
BUILD		?= build
OPT			?= -O2

ifeq ($(PLATFORM),web)
# Navigateur (make web) : emcc, SDL3 fourni par Emscripten, libft compilée ici
CC			:= emcc
LIBFT		:=
LIBFT_OBJ	:= $(patsubst %.c,$(BUILD)/%.o,$(wildcard libft/*.c))
SDL_CFLAGS	:= --use-port=sdl3
SDL_LIBS	:= --use-port=sdl3
WEB_FLAGS	:= $(OPT) -sALLOW_MEMORY_GROWTH -sSTACK_SIZE=1MB -sINVOKE_RUN=0 \
			   -sEXPORTED_RUNTIME_METHODS=callMain \
			   --shell-file web/shell.html \
			   --preload-file maps/good@/maps/good \
			   --preload-file textures@/textures --exclude-file '*.png'
else
LIBFT		:= libft/libft.a
LIBFT_OBJ	:=
SDL_CFLAGS	:= $(shell pkg-config --cflags sdl3 2>/dev/null)
SDL_LIBS	:= $(shell pkg-config --libs sdl3 2>/dev/null)
WEB_FLAGS	:=
endif

CFLAGS		:= -Wall -Wextra -Werror -MMD -MP $(OPT) -g $(EXTRA_CFLAGS) $(SDL_CFLAGS)
LDFLAGS		:= $(EXTRA_LDFLAGS) $(WEB_FLAGS)
LDLIBS		:= $(LIBFT) $(LIBFT_OBJ) $(SDL_LIBS) -lm

ENEMY_SRC	:= sources/enemy/enemy.c sources/enemy/enemy_move.c \
			   sources/enemy/enemy_view.c sources/enemy/enemy_spawn.c

LEVEL_SRC	:= sources/level/level.c sources/level/level_lines.c \
			   sources/level/level_header.c sources/level/level_map.c

SRC			:= sources/main.c sources/game.c sources/game_update.c \
			   sources/loop.c sources/options.c sources/stats.c sources/motion.c \
			   sources/raster.c sources/grid.c sources/raycast.c sources/pixels.c \
			   sources/font.c \
			   sources/weapon.c sources/waves.c sources/items.c \
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
			   $(BUILD)/tests/font_test $(BUILD)/tests/waves_test \
			   $(BUILD)/tests/items_test

ifeq ($(PLATFORM),web)
ifeq ($(shell command -v emcc 2>/dev/null),)
$(error emcc introuvable : installer Emscripten (https://emscripten.org/docs/getting_started/downloads.html) puis source emsdk_env.sh)
endif
endif

ifeq ($(filter clean fclean web,$(MAKECMDGOALS)),)
ifeq ($(SDL_LIBS),)
$(error SDL3 introuvable via pkg-config. macOS : brew install sdl3 | Linux récent : paquet libsdl3-dev | sinon : compiler SDL3 depuis https://github.com/libsdl-org/SDL)
endif
endif

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(LIBFT_OBJ) $(if $(WEB_FLAGS),web/shell.html)
	@mkdir -p $(dir $@)
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

# Version navigateur dans build-web/ : index.html, .js, .wasm et .data
web:
	$(MAKE) PLATFORM=web NAME=build-web/index.html BUILD=build-web/obj \
		OPT=-O2

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

$(BUILD)/tests/items_test: tests/items_test.c $(BUILD)/sources/items.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(filter %.c %.o,$^) -lm -o $@

clean:
	rm -rf build build-debug build-web
	$(MAKE) -C libft clean

fclean: clean
	rm -f cub3D cub3D_debug
	$(MAKE) -C libft fclean

re: fclean
	$(MAKE) all

.PHONY: all debug web test clean fclean re

-include $(OBJ:.o=.d) $(wildcard $(BUILD)/tests/*.d)
