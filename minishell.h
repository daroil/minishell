/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 17:52:49 by sbritani          #+#    #+#             */
/*   Updated: 2023/02/06 21:11:45 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "libft/libft.h"

# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/uio.h>


typedef struct dict_s
{
	char	**keys;
	char	**values;
	int		len;
}	t_dict;

typedef struct	settings_s
{
	int		last_exit_status;
	t_dict	*env;
	t_dict	*exported_env;
	char	*last_working_directory;
	char	*input;
}	t_settings;


typedef struct s_pipex
{
	char	**paths;
	int		**truby;
	int		fd;
	int		fd2;
	int		number_of_children;
	int		number_of_pipes;
	pid_t	*pids;
	int		here_doc;
	char	**cmds;
}	t_pipex;


char		*ft_str_join_free_first(char *str1, char *str2);
int			strings_equal(char *str1, char *str2);
t_settings	*create_setttings(char **env);
void		clear_settings(t_settings *settings);
char		**add_string_to_string_arr(char *str, char **arr, int n);
char		**remove_string_from_str_arr(char **arr, int n);
char		*str_copy(char *str, int n);
char		*ft_str_join_free_both(char *str1, char *str2);

// dict funcitons
t_dict	*init_dict(void);
void	dict_add(t_dict *dict, char *key, char *value);
char	*dict_get(t_dict *dict, char *key, char *default_return);
void	dict_delete(t_dict *dict, char *key);
void	free_dict(t_dict *dict);

// printing some stuff functions
void	print_dict(t_dict *dict);
void	print_splitted(char **splitted);

//echo functions
int	echo(char **args);

// split functions
typedef struct next_arg_return_s
{
	char	*arg;
	int		last_index;
}	t_next_arg_return;

int	is_bash_special_char(char c);
t_next_arg_return *get_next_arg(char *input, t_settings *settings);
char **split(char *input, t_settings *settings);

//split utils functions
t_next_arg_return	*init_next_arg(void);
void	free_next_arg_return(t_next_arg_return *next_arg);

//pipex funcitons
void	read_from_to(char *delimimter, int in_fd, int out_fd, int n_of_pipes);
char	*repeat_line_n_times(char *str, int n);
char	*join_three_lines(char *str1, char *str2, char *str3);
char	**get_paths(char **env);
char	*valid_path(char **paths, char *filename);
int		**make_truby(int n);
void	close_truby(int **truby, int cur, int len);
void	plumber(int **truby);
pid_t	*make_pids(int n);
void	waitress(pid_t *pids, int len);
char	**transform_args_to_cmds(char **argv);
void	dups(int fd1, int fd2);
int		pipex_init(t_pipex *pipex, char **argv, int argc, char **env);
void	clean_pipex(t_pipex *pipex);
void	error(t_pipex *pipex, char **args, char *path);
void	piping(t_pipex *pipex, int i, char **env);
char	**get_paths(char **env);
int	check_path(char **envp, char *splitted_input);
void	no_command(char **splitted_input, char *path, char **paths);
char	*valid_path(char **paths, char *filename);
int		single_pipe(char **splitted_input, int fd_in, int fd_out, char **envp);
// int		single_pipe_(char **cmd_and_args, int fd_in, int fd_out, char **envp, int **pip, pid_t	*pid, int num, int **pip2);
int	single_pipe_(char **cmd_and_args, int **truby, char **envp,pid_t	*pid, int num, int size);
char	*repeat_line_n_times(char *str, int n);

#endif