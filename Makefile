# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/17 18:04:41 by sbritani          #+#    #+#              #
#    Updated: 2023/02/12 16:20:13 by sbritani         ###   ########.fr        #
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
		my_ls.c \
		libft/libft.a

NAME =  shell
LIBFT = ./libft
OBJ	= $(SRCS:.c=.o)

INCRL		= -I /Users/$(USER)/.brew/opt/readline/include
LIBRL		= -lreadline -L /Users/$(USER)/.brew/opt/readline/lib
LIBRARIES	= -Llibft -lft $(LIBRL)
INCLUDES	= -I ./header -Ilibft $(INCRL)

all: libft $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) libft/libft.a $(LIBRL) $(INCRL) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCRL) -c $^ -o $@

clean:
	rm -f $(OBJ)

libft:
	@$(MAKE) -C $(LIBFT)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re libft