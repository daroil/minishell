# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/17 18:04:41 by sbritani          #+#    #+#              #
#    Updated: 2023/02/23 15:25:23 by sbritani         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc -Wall -Wextra -Werror

SRCS =	src/main.c\
		src/utils.c\
		src/settings.c\
		src/debugging.c\
		src/parse_input.c \
		src/split.c\
		src/resplit.c \
		src/split_utils.c\
		src/str_join_equal.c \
		src/handle_cases.c \
		src/handle_special_utils.c \
		src/signals.c \
		build_in/dict.c\
		build_in/cd_pwd.c \
		build_in/my_ls.c \
		build_in/echo.c\
		build_in/export_print.c\
		build_in/export_env_unset.c \
		pipex/change_in_out_put.c \
		pipex/pipes_utils.c \
		pipex/pipe_shell.c \
		pipex/pipex.c \
		pipex/pipex_utils.c \
		libft/libft.a

NAME =  minishell
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