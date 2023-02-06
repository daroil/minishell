/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 17:53:20 by sbritani          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/02/06 19:06:20 by sbritani         ###   ########.fr       */
=======
/*   Updated: 2023/02/06 18:38:30 by dhendzel         ###   ########.fr       */
>>>>>>> be6a3a7dbc7bfd796458663c43e125ba8a20b152
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *cur_dir(void)
{
	char *res;
	res = malloc(sizeof(char) * 100);
	if (!getcwd(res, 100))
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
	if (!splitted_input[1])
	{
		settings->last_working_directory = cur_dir();
		chdir("/");
		return (0);
	}
	if (strings_equal(splitted_input[1], "-\0"))
	{
		temp = settings->last_working_directory;
		settings->last_working_directory = cur_dir();
		chdir(temp);
		free(temp);
		return (0);
	}
	if (!access(splitted_input[1], X_OK))
	{
		free(settings->last_working_directory);
		settings->last_working_directory = cur_dir();
		chdir(splitted_input[1]);
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
		printf("sorry in current version env doesn't take any parameters\n");
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
			printf("tuta\n");
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

int parse_input(char *input, t_settings *settings,char **envp)
{
	char **splitted_input;
	pid_t	pid;
	int *pip;
	int *pip2;
	// char	**args;
	char	*path;
	char	**paths;
	splitted_input = split(input, settings);
	// print_splitted(splitted_input);

	// print_dict(settings->env);
	if (strings_equal(splitted_input[0], "exit"))
	{
		ft_split_clear(splitted_input);
		return (0);
	}
	if (!splitted_input[0])
	{
		ft_split_clear(splitted_input);
		return (1);
	}
	if (strings_equal(splitted_input[0], "echo\0"))
	{
		settings->last_exit_status = echo(splitted_input + 1);
		ft_split_clear(splitted_input);
		return (1);
	}
	if (strings_equal(splitted_input[0], "pwd\0"))
	{
		settings->last_exit_status = pwd(splitted_input);
		ft_split_clear(splitted_input);
		return (1);
	}
	if (strings_equal(splitted_input[0], "cd\0"))
	{
		settings->last_exit_status = cd(splitted_input, settings);
		ft_split_clear(splitted_input);
		return (1);
	}
	if (strings_equal(splitted_input[0], "env\0"))
	{
		settings->last_exit_status = env(splitted_input, settings);
		ft_split_clear(splitted_input);
		return (1);
	}
	if (strings_equal(splitted_input[0], "unset\0"))
	{
		settings->last_exit_status = unset(splitted_input, settings);
		ft_split_clear(splitted_input);
		return (1);
	}
	if (strings_equal(splitted_input[0], "export\0"))
	{
		settings->last_exit_status = export(splitted_input, settings);
		ft_split_clear(splitted_input);
		return (1);
	}
	if (ft_strchr(splitted_input[0], '='))
	{
		settings->last_exit_status = deal_with_equal_sign(splitted_input, settings);
		ft_split_clear(splitted_input);
		return (1);
	}
	else 
	{
		// int fd = open("testing", O_RDWR | O_CREAT | O_APPEND, 0644);
<<<<<<< HEAD
		// int *pip = malloc(sizeof(int) * 2);
		// char **inp = malloc(sizeof(char *) * 2);
		// inp[0] = str_copy("wc\0", -1); 
		// inp[1] = NULL;
		// pipe(pip);
		single_pipe_(splitted_input, 0, 1, envp);
=======
		pid_t	*pid;

		pid = malloc(sizeof(pid_t) * 3);
		pip = malloc(sizeof(int) * 2);
		pip2 = malloc(sizeof(int) * 2);
		char ***inp = malloc(sizeof(char **) * 4);
		inp[0] = malloc(sizeof(char *) * 3);
		inp[1] = malloc(sizeof(char *) * 3);
		inp[2] = malloc(sizeof(char *) * 3);
		inp[0][0] = str_copy("cat\0", -1);
		inp[0][1] = str_copy("Makefile\0", -1);
		inp[0][2] = NULL;
		inp[1][0] = str_copy("cat\0", -1);
		inp[1][1] = str_copy("Makefile\0", -1);
		inp[1][2] = NULL;
		inp[2][0] = str_copy("wc\0", -1);
		inp[2][1] = str_copy("-l\0", -1);
		inp[2][2] = NULL;
		inp[3] = NULL;
		pipe(pip);
		pipe(pip2);
		//if(needs pipe)
			//create pipe
			//while(number of pipes)
			//execute single pipe to pipe output
		single_pipe_(inp[0], 0, pip[1], envp, &pip, pid, 0, &pip2);
		single_pipe_(inp[1], pip[0], pip2[1], envp, &pip, pid, 1, &pip2);
		single_pipe_(inp[2], pip2[0], 1, envp, &pip, pid, 2, &pip2);
		usleep(1000);
		printf("%d %d %d %d\n", pip[0], pip[1], pip2[0], pip2[1]);
		close(pip[1]);
		printf("pervyi %s\n", get_next_line(pip[0]));
		close(pip[0]);
		close(pip2[1]);
		printf("vtoroi %s\n", get_next_line(pip2[0]));
		close(pip2[0]);
		waitpid(pid[0], NULL, 0);
		waitpid(pid[1], NULL, 0);
		waitpid(pid[2], NULL, 0);
		free(pip);
		free(pip2);
		free(pid);
		int clear = 0;
		while (inp[clear])
		{
			ft_split_clear(inp[clear]);
			clear++;
		}
		free(inp);
>>>>>>> be6a3a7dbc7bfd796458663c43e125ba8a20b152
		// printf("finished pipe\n");
		// close(pip[1]);
		// single_pipe(inp, pip[0], STDOUT_FILENO, envp);
		// close(pip[0]);
		// pid = fork();
		// if (!pid)
		// {
		// 	paths = get_paths(envp);
		// 	path = valid_path(paths, splitted_input[0]);
		// 	if (!path)
		// 		no_command(splitted_input, path, paths);
		// 	execve(path, splitted_input, envp);
		// }	
		// waitpid(pid, NULL, 0);
		ft_split_clear(splitted_input);
		return (1);
	}
	// else
	// {
	// 	printf("%s is an unknown command\n", splitted_input[0]);
	// 	settings->last_exit_status = 127;
	// 	ft_split_clear(splitted_input);
	// 	return (1);
	// }
}


void	finish(t_settings *settings, char *input)
{
	clear_settings(settings);
	free(input);
}

void	my_readline(t_settings *settings);

void	interrupt_input(int sig)
{
	// printf("\nint interrupt 1\n");
	rl_on_new_line();
	rl_replace_line("\0", 0);
	printf("\n");
	rl_redisplay();
	// printf("int interrupt 2\n");
	// my_readline(NULL);
}

void	my_readline(t_settings *settings)
{
	char *prompt;
	static t_settings *local_settings = NULL;
	
	if (!local_settings)
		local_settings = settings;
	prompt = ft_str_join_free_first(cur_dir(), "> \0");
	local_settings->input = readline(prompt);
	free(prompt);
}

void	shell(char *envp[])
{
	char *res;
	t_settings *settings;

	settings = create_setttings(envp);
	settings->last_working_directory = cur_dir();
	signal(SIGINT, interrupt_input);
	my_readline(settings);
	if (!parse_input(settings->input, settings, envp))
		return (finish(settings, settings->input));
	while (settings->input)
	{
		add_history(settings->input);
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