# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/17 18:04:41 by sbritani          #+#    #+#              #
#    Updated: 2023/02/03 15:51:56 by dhendzel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc

SRCS =	main.c\
		utils.c\
		settings.c\
		dict.c\
		debugging.c\
		split.c\
		split_utils.c\
		echo.c\
		pipe_shell.c \
		libft/libft.a

NAME =  shell
LIBFT = ./libft
OBJ	= $(SRCS:.c=.o)


all: libft $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) libft/libft.a -lreadline -o $(NAME)

clean:
	rm -f $(OBJ)

libft:
	@$(MAKE) -C $(LIBFT)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re libft