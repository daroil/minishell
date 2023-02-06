#!/bin/sh
clear
SRC="main.c utils.c settings.c dict.c debugging.c split.c split_utils.c echo.c pipe_shell.c libft/libft.a"
gcc -I /Users/$USER/.brew/opt/readline/include -lreadline -L /Users/$USER/.brew/opt/readline/lib $SRC
./a.out