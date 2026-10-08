NAME		?= cub3D
BUILD		?= build
OPT			?= -O2

LIBFT		:= libft/libft.a
SDL_CFLAGS	:= $(shell pkg-config --cflags sdl3 2>/dev/null)
SDL_LIBS	:= $(shell pkg-config --libs sdl3 2>/dev/null)

CFLAGS		:= -Wall -Wextra -Werror -MMD -MP $(OPT) -g $(EXTRA_CFLAGS) $(SDL_CFLAGS)
LDFLAGS		:= $(EXTRA_LDFLAGS)
LDLIBS		:= $(LIBFT) $(SDL_LIBS) -lm

SRC			:= sources/main.c sources/init.c \
			   sources/loop.c sources/options.c sources/stats.c sources/motion.c \
			   sources/raster.c sources/grid.c sources/raycast.c \
			   sources/platform/platform_sdl.c \
			   sources/platform/xpm_loader.c \
			   sources/drawing/minimap.c \
			   sources/drawing/drawing.c sources/drawing/drawing2.c \
			   sources/drawing/player.c \
			   sources/drawing/utils.c \
			   sources/parser/ft_flood_fill.c sources/parser/get_scene_data.c \
			   sources/parser/get_colors.c sources/parser/file_check.c \
			   sources/parser/get_textures.c sources/parser/parse_map.c \
			   sources/parser/extract_map.c sources/parser/utils_2dstr.c
OBJ			:= $(SRC:%.c=$(BUILD)/%.o)

TEST_BINS	:= $(BUILD)/tests/xpm_test $(BUILD)/tests/options_test \
			   $(BUILD)/tests/stats_test $(BUILD)/tests/motion_test \
			   $(BUILD)/tests/raster_test $(BUILD)/tests/raycast_test

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
