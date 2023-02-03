#!/bin/sh
clear
SRC="libft/libft.a tests.c minishell.h split.c split_utils.c dict.c utils.c settings.c"
gcc $SRC
./a.out