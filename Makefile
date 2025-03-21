NAME = cub3D
HEADER = includes/cub3d.h 
SRC =  main.c init.c
SRC := $(addprefix sources/, $(SRC))
SRC_DRW = bonus_map.c bonus_map2.c drawing2.c drawing.c player.c \
		player_key.c player_mouv.c utils2.c utils.c utils_math.c
SRC_DRW := $(addprefix sources/drawing/, $(SRC_DRW))
PRS_SRC = ft_flood_fill.c get_scene_data.c get_colors.c get_textures.c \
		parse_map.c extract_map.c utils_2dstr.c
PRS_SRC := $(addprefix sources/parser/, $(PRS_SRC))
SRC += $(SRC_DRW)
SRC += $(PRS_SRC)
OBJ = ${SRC:%.c=%.o}
MLX_DIR = ./minilibx-linux
MLX_FLG = -lmlx -lX11 -lXext -lm
MLX = ./minilibx-linux/libmlx_Linux.a
LIBFT = ./libft/libft.a

all : $(NAME) 

$(LIBFT) :
	@echo "<Compiling libft>"
	@make -C ./libft

$(MLX) :
	@echo "<Compiling minilibx>"
	@make -C ./minilibx-linux

%.o: %.c Makefile $(LIBFT) $(MLX)
	@cc -c -g -Werror -Wall -Wextra $< -o $@ 

$(NAME) : $(HEADER) $(LIBFT) $(OBJ) Makefile
	@echo "Creating the program <cub3D>"
	@cc -g -Werror -Wall -Wextra -L$(MLX_DIR) $(OBJ) $(MLX_FLG) $(LIBFT) $(MLX) -Iinclude -ldl -lglfw -lm -o $(NAME)

%.o: %.c
	$(CC) -g $(CFLAGS) -I$(MLX_DIR) -c $< -o $@

clean :
	@echo "Removing object files"
	@rm -f $(OBJ)
	@make clean -C ./libft	
	@make clean -C ./minilibx-linux

fclean : clean 
	@echo "Removing the executable <cub3D>"
	@rm -f cub3D
	@make fclean -C ./libft

re : fclean all

.PHONY: all, clean, fclean, re
