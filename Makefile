NAME = cub3d

CC = gcc

CFLAGS = -Wall -Wextra -Werror

MLX_DIR = ./minilibx-linux

MLX = -lmlx -lX11 -lXext -lm

SRC = sources/*.c

LIBFT	=	libft/libft.a

OBJ = $(patsubst %.c,%.o,$(wildcard $(SRC)))

all: $(NAME)

$(LIBFT):
	@make -C libft

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) -L$(MLX_DIR) $(OBJ) $(MLX) -Llibft -lft -o $(NAME)

%.o: %.c
	$(CC) -g $(CFLAGS) -I$(MLX_DIR) -c $< -o $@

clean:
	rm -f $(OBJ)
	@make clean -C libft

fclean: clean
	rm -f $(NAME)
	@make fclean -C libft

re: fclean all

.PHONY: all clean fclean re
