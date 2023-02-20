/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 17:53:20 by sbritani          #+#    #+#             */
/*   Updated: 2023/02/20 14:32:43 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

char *cur_dir(void)
{
	char *res;
	res = malloc(sizeof(char) * 1000);
	if (!getcwd(res, 1000))
	{
		free(res);
		return (NULL);
	}
	return (res);
}

int	pwd(char **splitted_input)
{
	char	*to_print;
	if (splitted_input[1])
		printf("pwd: too many arguments\n");
	else
	{
		to_print = cur_dir();
		printf("%s\n",to_print);
		free(to_print);
	}
	return (0);
}

int cd(char **splitted_input, t_settings *settings)
{
	char *temp;
	char *other_temp;
	if (!splitted_input[1])
	{
		settings->last_working_directory = cur_dir();
		chdir("/");
		dict_add(settings->exported_env, "OLDPWD\0",settings->last_working_directory);
		other_temp = cur_dir();
		dict_add(settings->exported_env, "PWD\0", other_temp);
		free(other_temp);
		return (0);
	}
	if (strings_equal(splitted_input[1], "-\0"))
	{
		temp = settings->last_working_directory;
		settings->last_working_directory = cur_dir();
		chdir(temp);
		dict_add(settings->exported_env, "OLDPWD\0", settings->last_working_directory);
		other_temp = cur_dir();
		dict_add(settings->exported_env, "PWD\0", other_temp);
		free(other_temp);
		free(temp);
		return (0);
	}
	if (!access(splitted_input[1], X_OK))
	{
		free(settings->last_working_directory);
		settings->last_working_directory = cur_dir();
		chdir(splitted_input[1]);
		dict_add(settings->exported_env, "OLDPWD\0", settings->last_working_directory);
		other_temp = cur_dir();
		dict_add(settings->exported_env, "PWD\0", other_temp);
		free(other_temp);
		return (0);
	}
	printf("cd: no such file or directory: %s\n", splitted_input[1]);
	return (1);
}

int	env(char **splitted_input, t_settings *settings)
{
	int	i;

	i = 0;
	if (splitted_input[1])
	{
		printf("sorry in current version env doesn't take any  ameters\n");
		return (127);
	}
	while (settings->exported_env->keys[i])
	{
		if (settings->exported_env->values[i])
			printf("%s=%s\n", settings->exported_env->keys[i], settings->exported_env->values[i]);
		i++;
	}
	return (0);
}

int	unset(char **splitted_input, t_settings *settings)
{
	int	i;

	i = 1;
	while (splitted_input[i])
	{
		dict_delete(settings->env, splitted_input[i]);
		dict_delete(settings->exported_env, splitted_input[i]);
		i++;
	}
	return (0);
}

char **split_for_equal_sign(char *str)
{
	char	**res;
	int	i;

	res = malloc(sizeof(char *) * 3);
	res[2] = NULL;
	while (str[i] && str[i] != '=')
		i++;
	res[0] = str_copy(str, i);
	res[1] = str_copy(str + i + 1, -1);
	if (!res[1][0])
	{
		free(res[1]);
		res[1] = NULL;
	}
	return (res);
}

int	export(char **splitted_input, t_settings *settings)
{
	int		i;
	char	**temp;

	i = 1;
	while(splitted_input[i])
	{
		temp = split_for_equal_sign(splitted_input[i]);
		// print_splitted(temp);
		if (temp[1])
		{
			dict_add(settings->env, temp[0], temp[1]);
			dict_add(settings->exported_env, temp[0], temp[1]);
		}
		else if (dict_get(settings->env, temp[0], NULL))
		{
			dict_add(settings->exported_env, temp[0], dict_get(settings->env, temp[0], NULL));
		}
		else
			dict_add(settings->exported_env, temp[0], NULL);
		ft_split_clear(temp);
		i++;
	}
	return (0);
}


int	deal_with_equal_sign(char **splitted_input, t_settings *settings)
{
	int	i;
	char **temp;

	while (splitted_input[i])
	{
		temp = split_for_equal_sign(splitted_input[i]);
		if (temp[1])
			dict_add(settings->env, temp[0], temp[1]);
		else
		{
			ft_split_clear(temp);
			return (127);
		}
		ft_split_clear(temp);
		i++;
	}
	return (0);
}

char	*valid_path(char **paths, char *filename)
{
	char	*full_path;
	char	*path_with_slash;
	int		i;

	i = -1;
	if (!paths)
		return(NULL);
	if (!access(filename, X_OK))
		return (filename);
	while (paths[++i])
	{
		path_with_slash = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path_with_slash, filename);
		free(path_with_slash);
		if (!access(full_path, X_OK))
			return (full_path);
		free(full_path);
	}
	return (NULL);
}

char	**get_paths(char **env)
{
	int		i;
	char	**mid_res;

	i = -1;
	mid_res = NULL;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], "PATH", 4))
			mid_res = ft_split(&env[i][5], ":");
	}
	return (mid_res);
}

int	check_path(char **envp, char *splitted_input)
{
	char **paths;
	char *path;
	paths = get_paths(envp);
	path = valid_path(paths, splitted_input);
	ft_split_clear(paths);
	if (!path)
	{
		free(path);
		return(0);		
	}
	free(path);
	return (1);
}

int	string_in_array_of_strings(char *string, char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if (strings_equal(array[i], string))
			return(1);
		i++;
	}
	return (0);
}

void	no_command(char **splitted_input, char *path, char **paths)
{
	ft_putstr_fd(splitted_input[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror("command not found");
	ft_split_clear(paths);
	free(path);
	ft_split_clear(splitted_input);
	exit (127);
}

int	check_angulars(char **splitted_input)
{
	int	i;
	
	i = 0;
	while (splitted_input[i])
	{
		if (splitted_input[i][0] == '>' || splitted_input[i][0] == '<')
		{
			if (!splitted_input[i + 1])
			{
				printf("syntax error near unexpected token 'newline'\n");
				return (0);
			}
			if (splitted_input[i + 1][0] == '|')
			{
				printf("syntax error near unexpected token '|'\n");
				return (0);
			}
			if (splitted_input[i + 1][0] == '<')
			{
				printf("syntax error near unexpected token '<'\n");
				return (0);
			}
			if (splitted_input[i + 1][0] == '>')
			{
				printf("syntax error near unexpected token '>'\n");
				return (0);
			}
		}
		i++;
	}
	return (1);
}

void clear_splits(char **splitted_input, char ***resplitted_input)
{
	ft_split_clear(splitted_input);
	free_resplitted(resplitted_input);
}

int check_basic_commands(char **splitted_input, char ***resplitted_input, t_settings *settings)
{
	if (strings_equal(splitted_input[0], "exit"))
		return (1);
	if (!splitted_input[0])
		return (1);
	if (strings_equal(splitted_input[0], "echo\0"))
		return (1);
	if (strings_equal(splitted_input[0], "pwd\0"))
		return (1);
	if (strings_equal(splitted_input[0], "cd\0"))
		return (1);
	if (strings_equal(splitted_input[0], "env\0"))
		return (1);
	if (strings_equal(splitted_input[0], "unset\0"))
		return (1);
	if (strings_equal(splitted_input[0], "export\0"))
		return (1);
	if (ft_strchr(splitted_input[0], '='))
		return (1);
	return(0);
}

int basic_commands(char **splitted_input, char ***resplitted_input, t_settings *settings)
{
	if (strings_equal(splitted_input[0], "exit"))
		return (clear_splits(splitted_input, resplitted_input), 0);
	else if (!splitted_input[0])
		return (clear_splits(splitted_input, resplitted_input), 1);
	else if (strings_equal(splitted_input[0], "echo\0"))
		settings->last_exit_status = echo(splitted_input + 1);
	else if (strings_equal(splitted_input[0], "pwd\0"))
		settings->last_exit_status = pwd(splitted_input);
	else if (strings_equal(splitted_input[0], "cd\0"))
		settings->last_exit_status = cd(splitted_input, settings);
	else if (strings_equal(splitted_input[0], "env\0"))
		settings->last_exit_status = env(splitted_input, settings);
	else if (strings_equal(splitted_input[0], "unset\0"))
		settings->last_exit_status = unset(splitted_input, settings);
	else if (strings_equal(splitted_input[0], "export\0"))
		settings->last_exit_status = export(splitted_input, settings);
	else if (ft_strchr(splitted_input[0], '='))
		settings->last_exit_status = deal_with_equal_sign(splitted_input, settings);
	return (clear_splits(splitted_input, resplitted_input), 1);
}

void	pipex_init(t_settings *settings, char ***resplitted_input)
{
	change_ctrl_c();
	settings->pipex->number_of_pipes = count_resplitted(resplitted_input) - 1;
	settings->pipex->pid = malloc(sizeof(pid_t) * (settings->pipex->number_of_pipes + 1));
	settings->pipex->truby = malloc(sizeof(int *) * (settings->pipex->number_of_pipes + 1));
	settings->pipex->i = 0;
	while (settings->pipex->i < settings->pipex->number_of_pipes)
	{
		settings->pipex->truby[settings->pipex->i] = malloc(sizeof(int) * 2);
		pipe(settings->pipex->truby[settings->pipex->i]);
		settings->pipex->i++;
	}
	settings->pipex->truby[settings->pipex->i] = NULL;
}

void	clean_and_wait_pipex(t_settings *settings)
{
	settings->pipex->i = 0;
	while(settings->pipex->truby[settings->pipex->i])
	{
		close(settings->pipex->truby[settings->pipex->i][0]);
		close(settings->pipex->truby[settings->pipex->i][1]);
		free(settings->pipex->truby[settings->pipex->i]);
		settings->pipex->i++;
	}
	free(settings->pipex->truby);
	settings->pipex->i = 0;
	while (settings->pipex->i <= settings->pipex->number_of_pipes)
	{
		waitpid(settings->pipex->pid[settings->pipex->number_of_pipes], NULL, 0);
		settings->pipex->i++;
	}
	free(settings->pipex->pid);
}

int pipex(char **splitted_input, char ***resplitted_input, t_settings *settings)
{
	t_pipex pipex;
	char **something;
	
	something = unite_env(settings->exported_env); 
	settings->pipex = &pipex;
	pipex_init(settings, resplitted_input);
	pipex.i = 0;
	while (pipex.i <= pipex.number_of_pipes)
	{
		single_pipe(resplitted_input[pipex.i], pipex, something, settings);
		if (string_in_array_of_strings("<<", resplitted_input[pipex.i]))
			waitpid(pipex.pid[pipex.i], NULL, 0);
		pipex.i++;
	}
	clean_and_wait_pipex(settings);
	disable_ctrlc();
	ft_split_clear(splitted_input);
	ft_split_clear(something);
	free_resplitted(resplitted_input);
	settings->pipex = NULL;
	return (1);
}

int parse_input(char *input, t_settings *settings,char **envp)
{
	char **splitted_input;
	char ***resplitted_input;
	if (!input)
		return (0);
	splitted_input = split(input, settings);
	if (!check_angulars(splitted_input))
	{
		ft_split_clear(splitted_input);
		return (1);
	}
	resplitted_input = resplit(splitted_input);
	if (count_resplitted(resplitted_input) == 1)
	{
		if((check_basic_commands(splitted_input, resplitted_input, settings)))
			return(basic_commands(splitted_input, resplitted_input, settings));
		else 
			return(pipex(splitted_input, resplitted_input, settings));
	}
	else 
		return(pipex(splitted_input, resplitted_input, settings));
}


void	finish(t_settings *settings, char *input)
{
	clear_settings(settings);
	free(input);
}

void	my_readline(t_settings *settings);

void	kill_children(t_settings *settings, int to_kill, int sig)
{
	static t_settings *local_settings = NULL;
	int i;

	if (!local_settings)
		local_settings = settings;
	if (to_kill && local_settings->pipex && local_settings->pipex->pid)
	{
		if (sig == SIGQUIT)
			printf("Quit: 3\n");
		i = 0;
		while (local_settings->pipex->pid[i])
			{
				kill(local_settings->pipex->pid[i], sig);
				i++;
			}
	}
}

void	interrupt_input(int sig)
{
	// printf("\nint interrupt 1\n");
	kill_children(NULL, 1, sig);
	if (sig == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("\0", 0);
		printf("\n");
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		rl_redisplay();
		printf("\b");
	}
	// printf("int interrupt 2\n");
	// my_readline(NULL);
}

void	my_readline(t_settings *settings)
{
	char *prompt;
	static t_settings *local_settings = NULL;
	
	if (!local_settings)
		local_settings = settings;
	prompt = ft_str_join_free_both(str_copy("\r\0", -1), ft_str_join_free_first(cur_dir(), "> \0"));
	// prompt = ft_str_join_free_first(cur_dir(), "> \0");
	local_settings->input = readline(prompt);
	free(prompt);
}

struct termios saved;

void restore(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &saved);
}

void	disable_ctrlc(void)
{
	struct	termios term;
	tcgetattr(STDIN_FILENO, &term);
	if (term.c_lflag & ECHOCTL)
		term.c_lflag ^= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}

void	change_ctrl_c(void)
{
	struct	termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag ^= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}
void	enable_ctrlc(void)
{
	printf("1\n");
	static struct termios* term_state = NULL;
	printf("2\n");
	if (!term_state)
		term_state = malloc(sizeof(struct termios *));
	printf("3\n");
	tcgetattr(STDIN_FILENO,term_state);
	printf("4\n");
	term_state->c_lflag = ECHOCTL | ECHO | ECHOE | ECHOKE | ICANON | ISIG | IEXTEN | PENDIN;
	printf("5\n");
    tcsetattr(STDIN_FILENO, TCSAFLUSH, term_state);
	printf("6\n");
	// free(term_state);
	printf("7\n");
}


void	shell(char *envp[])
{
	char *res;
	t_settings *settings;

	settings = create_setttings(envp);
	kill_children(settings, 0, 0);
	disable_ctrlc();
	settings->last_working_directory = cur_dir();
	signal(SIGINT, interrupt_input);
	signal(SIGQUIT, interrupt_input);
	my_readline(settings);
	if (!parse_input(settings->input, settings, envp))
		return (finish(settings, settings->input));
	while (settings->input)
	{
		if (!strings_equal(settings->last_cmd, settings->input))
			add_history(settings->input);
		if (settings->input)
		{
			if (settings->last_cmd)
			{
				free(settings->last_cmd);
				settings->last_cmd = NULL;
			}
			settings->last_cmd = str_copy(settings->input, -1);
		}
		free(settings->input);
		my_readline(NULL);
		if (!parse_input(settings->input, settings, envp))
			return (finish(settings, settings->input));
	}
	finish(settings, settings->input);
}



int main(int argc, char **argv, char **envp)
{
	if (argc && argv)
	shell(envp);
	system("leaks shell");
	// return(0);
}