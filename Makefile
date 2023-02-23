# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/17 18:04:41 by sbritani          #+#    #+#              #
#    Updated: 2023/02/23 03:43:22 by dhendzel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc

SRCS =	main.c\
		utils.c\
		settings.c\
		dict.c\
		debugging.c\
		split.c\
		resplit.c \
		split_utils.c\
		echo.c\
		my_ls.c \
		str_join_equal.c \
		handle_cases.c \
		handle_special_utils.c \
		pipe_shell.c \
		change_in_out_put.c \
		pipes_utils.c \
		parse_input.c \
		cd_pwd.c \
		export_env_unset.c \
		pipex.c \
		pipex_utils.c \
		signals.c \
		libft/libft.a

NAME =  shell
LIBFT = ./libft
OBJ	= $(SRCS:.c=.o)

INCRL		= -I /Users/$(USER)/.brew/opt/readline/include
LIBRL		= -lreadline -L /Users/$(USER)/.brew/opt/readline/lib
LIBRARIES	= -Llibft -lft $(LIBRL)
INCLUDES	= -I ./header -Ilibft $(INCRL)

# INCRL = -I$(DLDIR)/readline_out/include -Ilibft -Iincludes
# LIBRL = -L$(DLDIR)/readline_out/lib -lreadline


all: libft $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) libft/libft.a $(LIBRL) $(INCRL) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCRL) -c $^ -o $@

clean:
	@$(MAKE) -C $(LIBFT) clean
	rm -f $(OBJ)

libft:
	@$(MAKE) -C $(LIBFT)

fclean: clean
	@$(MAKE) -C $(LIBFT) fclean
	rm -f $(NAME)

re: fclean relib all
	
relib:
	@$(MAKE) -C $(LIBFT) re
	
.PHONY: all clean fclean re libft relib