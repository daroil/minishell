/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 17:52:49 by sbritani          #+#    #+#             */
/*   Updated: 2023/02/23 13:58:35 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include "libft/libft.h"
# include <termios.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <dirent.h>
# include <signal.h>

typedef struct dict_s
{
	char	**keys;
	char	**values;
	int		len;
}	t_dict;

typedef struct s_pipex
{
	pid_t	*pid;
	int		number_of_pipes;
	int		**truby;
	int		i;
	int		redirect_input;
	int		redirect_output;
}	t_pipex;

typedef struct settings_s
{
	int				last_exit_status;
	t_dict			*env;
	t_dict			*exported_env;
	char			*last_working_directory;
	char			*input;
	char			*last_cmd;
	struct termios	*term_state;
	t_pipex			*pipex;
}	t_settings;

typedef struct next_arg_return_s
{
	char	*arg;
	int		last_index;
}	t_next_arg_return;

char				*ft_str_join_free_first(char *str1, char *str2);
void				free_two_strings(char *str1, char *str2);
int					strings_equal(char *str1, char *str2);
t_settings			*create_setttings(char **env);
void				clear_settings(t_settings *settings);
char				**add_string_to_string_arr(char *str, char **arr, int n);
char				**remove_string_from_str_arr(char **arr, int n);
char				*str_copy(char *str, int n);
char				*ft_str_join_free_both(char *str1, char *str2);

//parsing input
int					parse_input(char *input, t_settings *settings);
int					check_basic_commands(char **splitted_input);
int					basic_commands(char **splitted_input,
						char ***resplitted_input, t_settings *settings);
int					check_angulars(char **splitted_input);
int					check_pipe(char **splitted_input);

//signal functions
void				disable_ctrlc(void);
void				enable_ctrlc(void);
int					kill_children(t_settings *settings,
						int to_kill, int sig);
void				interrupt_input(int sig);

//basic shell functions
int					deal_with_equal_sign(char **splitted_input,
						t_settings *settings);
int					export(char **splitted_input, t_settings *settings);
int					unset(char **splitted_input, t_settings *settings);
int					env(char **splitted_input, t_settings *settings);
int					cd(char **splitted_input, t_settings *settings);
char				*cur_dir(void);
int					cd_home(t_settings *settings);
int					cd_minus(t_settings *settings);
int					pwd(char **splitted_input);
char				**split_for_equal_sign(char *str);
void				free_last_cmd(t_settings *settings);

// dict funcitons
t_dict				*init_dict(void);
void				dict_add(t_dict *dict, char *key, char *value);
char				*dict_get(t_dict *dict, char *key, char *default_return);
void				dict_delete(t_dict *dict, char *key);
void				free_dict(t_dict *dict);
char				**unite_env(t_dict *dict);

// printing some stuff functions
void				print_dict(t_dict *dict);
void				print_splitted(char **splitted);
void				print_resplitted(char ***resplitted);

// changing stty
void				change_ctrl_c(void);

//echo functions
int					echo(char **args);

// split functions

int					is_bash_special_char(char c);
t_next_arg_return	*get_next_arg(char *input, t_settings *settings);
char				**split(char *input, t_settings *settings);
char				***resplit(char **splitted);
void				free_resplitted(char ***resplitted);
int					count_resplitted(char ***resplitted);
t_next_arg_return	*handle_spec_start(t_settings *settings,
						t_next_arg_return *res, char *input, int start);
t_next_arg_return	*handle_regular(char *input, int start,
						t_settings *settings, t_next_arg_return *res);
int					for_spec_char_return_index(char *input,
						t_next_arg_return *res, t_settings *settings, int i);
void				start_with_more(char *input, int start,
						t_next_arg_return *res);
void				start_with_less(char *input, int start,
						t_next_arg_return *res);
void				start_with_pipe(int start,
						t_next_arg_return *res);
int					dollar_or_quote(char *input, int i);
t_next_arg_return	*deal_with_single_quotes(char *input);
t_next_arg_return	*deal_with_double_quotes(char *input, t_settings *settings);
t_next_arg_return	*no_input(t_next_arg_return	*res, int i);
int					dollar_or_quote(char *input, int i);
int					start_with_spec(char *input, int start);

//resplit functions
char				***resplit(char **splitted);
int					count_resplitted(char ***resplitted);
void				free_resplitted(char ***resplitted);
char				***add_string_array_to_array_of_string_arrays(char ***old,
						char **to_add);
char				**copy_str_array(char **array, int n);

//split utils functions
t_next_arg_return	*init_next_arg(void);
void				free_next_arg_return(t_next_arg_return *next_arg);
t_next_arg_return	*deal_with_dollar(char *input, t_settings *settings);
void				clear_splits(char **splitted_input,
						char ***resplitted_input);

//pipex funcitons
void				read_from_to_shell(char *delimimter, int in_fd, int out_fd);
char				*repeat_line_n_times(char *str, int n);
char				*join_three_lines(char *str1, char *str2, char *str3);
char				**get_paths(char **env);
char				*valid_path(char **paths, char *filename);
int					**make_truby(int n);
void				close_truby(int **truby, int cur, int len);
void				plumber(int **truby);
pid_t				*make_pids(int n);
void				waitress(pid_t *pids, int len);
char				**transform_args_to_cmds(char **argv);
void				error(t_pipex *pipex, char **args, char *path);
void				piping(t_pipex *pipex, int i, char **env);
char				**get_paths(char **env);
int					check_path(char **envp, char *splitted_input);
void				no_command(char **splitted_input, char *path, char **paths);
char				*valid_path(char **paths, char *filename);
int					single_pipe(char **cmd_and_args, t_pipex pipex,
						char **envp, t_settings *settings);
void				interrupt_input_doc(int signal);
int					outfile_change_append(t_pipex *pipex, char **cmd,
						int i, char **cmd_and_args);
int					outfile_change(t_pipex *pipex, char **cmd,
						int i, char **cmd_and_args);
int					infile_heredoc(t_pipex *pipex, char **cmd,
						int i, char **cmd_and_args);
int					array_len(char **array);
int					infile_change(t_pipex *pipex, char **cmd,
						int i, char **cmd_and_args);
void				clean_exit(char **cmd);
int					pipex(char **splitted_input, char ***resplitted_input,
						t_settings *settings);
void				clean_and_wait_pipex(t_settings *settings);
void				pipex_init(t_settings *settings, char ***resplitted_input);
int					string_in_array_of_strings(char *string, char **array);
char				**unite_env(t_dict *dict);

// wild stuf
int					matches_wild(char *str, char *wild);
char				**my_ls(void);
char				**add_wild_matches_if_needed(char **splitted_input,
						int len_splitted);

#endif