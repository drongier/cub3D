# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mekundur <mekundur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/16 19:34:04 by mekundur          #+#    #+#              #
#    Updated: 2025/03/04 16:56:37 by mekundur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
HEADER = libft.h
HEADERGNL = get_next_line.h
SOURCES = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	  ft_tolower.c ft_toupper.c ft_atoi.c ft_atoi_hex.c ft_strncmp.c ft_memcmp.c \
	  ft_strlen.c ft_strlcpy.c ft_strlcat.c ft_strchr.c ft_strrchr.c \
	  ft_itoa.c ft_strdup.c ft_strnstr.c ft_substr.c ft_strjoin.c \
	  ft_strtrim.c ft_split.c ft_strmapi.c ft_memset.c ft_bzero.c \
	  ft_memcpy.c ft_memmove.c ft_putchar_fd.c ft_putstr_fd.c \
	  ft_putendl_fd.c ft_putnbr_fd.c ft_memchr.c ft_calloc.c ft_striteri.c \
	  ft_lstsize.c ft_lstadd_front.c ft_lstadd_back.c ft_lstdelone.c \
	  ft_lstclear.c ft_lstiter.c ft_lstnew.c ft_lstlast.c ft_lstmap.c \
	  get_next_line.c get_next_line_utils.c ft_split_wspaces.c \
	  ft_printf.c ft_printf_hexflags.c ft_printf_nbrflags.c \
	  ft_printf_putchar.c ft_printf_puthex.c ft_printf_putnbr.c \
	  ft_printf_putptr.c ft_printf_putstr.c ft_printf_strutils.c \
	  ft_printfutils.c ft_iswhitespace.c ft_isemptyline.c

OBJECTS = $(SOURCES:.c=.o)

all : $(NAME) 

%.o: %.c Makefile $(HEADER) $(HEADERGNL)
	@cc -c -g -Werror -Wall -Wextra $< -o $@

$(NAME) : $(OBJECTS) $(HEADER) $(HEADERGNL)
	@echo "Creating the library <libft.a>"
	@ar -rcs $@ $(OBJECTS)

clean :
	@echo "Cleaning up object files"
	@rm -f $(OBJECTS)

fclean : clean 
	@echo "Removing the library <libft.a>"
	@rm -f libft.a

re : fclean all

.PHONY: all, clean, fclean, re
