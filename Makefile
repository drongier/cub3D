NAME = cub3D
LIBFT = ./libft/libft.a
MLX42 = ./MLX42/build/libmlx42.a
SRC =  main.c debug_bonus.c drawing2.c drawing.c player.c player_key.c player_mouv.c utils2.c utils.c utils_math.c
SRC := $(addprefix sources/, $(SRC))
PRS_SRC = ft_flood_fill.c get_scene_data.c parse_map.c utils_2dstr.c
PRS_SRC := $(addprefix sources/parser/, $(PRS_SRC))
SRC += $(PRS_SRC)
OBJ = ${SRC:%.c=%.o}

all : $(NAME) 

$(LIBFT) :
	@echo "<Compiling libft>"
	@make -C ./libft

%.o: %.c Makefile $(LIBFT)
	@cc -c -g -Werror -Wall -Wextra $< -o $@ 

MLX_DIR = ./minilibx-linux
MLX = -lmlx -lX11 -lXext -lm

$(NAME) : $(HEADER) $(LIBFT) $(OBJ) Makefile
	@echo "Creating the program <cub3D>"
	@cc -g -Werror -Wall -Wextra -L$(MLX_DIR) $(OBJ) $(MLX) $(LIBFT) -Iinclude -ldl -lglfw -lm -o $(NAME)


#$(NAME): $(LIBFT) $(OBJ)
#	$(CC) $(CFLAGS) -L$(MLX_DIR) $(OBJ) $(MLX) -Llibft -lft -o $(NAME)

%.o: %.c
	$(CC) -g $(CFLAGS) -I$(MLX_DIR) -c $< -o $@

clean :
	@echo "Removing object files"
	@rm -f $(OBJ)
	@make clean -C ./libft	
#	@make clean -C ./MLX42/build

fclean : clean 
	@echo "Removing the executable <cub3D>"
	@rm -f cub3D
	@make fclean -C ./libft

re : fclean all

.PHONY: all, clean, fclean, re
