/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 15:45:27 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/03 19:43:04 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dups(int fd1, int fd2)
{
	dup2(fd1, STDIN_FILENO);
	dup2(fd2, STDOUT_FILENO);
}

int	is_arrow(char c)
{
	if (c == '<' || c == '>')
		return(1);
	return(0);
}

int	count_arrows(char *line)
{
	int	i;
	int	count;
	i = 0;
	count = 0;
	while (line[i])
	{
		if (is_arrow(line[i]))
			count++;
		if (is_arrow(line[i+1]))
			i++;
		i++;
	}
	return(count);
}

static int	count_words(char const *s, char *c)
{
	int		counter;
	int		i;
	int		prev;

	prev = 0;
	counter = 0;
	i = 0;
	while (s[i])
	{
		if (is_arrow(s[i]))
		{
			counter += prev;
			prev = 0;
		}
		else
			prev = 1;
		i++;
	}
	if (i == 0)
		return (0);
	return (counter + (!ft_strchr(c, s[i - 1])));
}

// int	has_arrows(const char *s)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i])
// 	{
// 		if ()
// 		i++;
// 	}
// 	return (0);
// }

char	**split_by_arrows(char *command_args)
{
	char	**result;
	int		i;
	int		new_word_start;
	int		counter;
	int		word;
	int		size;

	size = count_arrows(command_args);
	result = malloc(sizeof(char *) * (size + 1));
	// result[count_arrows(command_args)] = NULL;
	i = 0;
	new_word_start = 0;
	word = -1;
	while (++word < size)
	{
		while (is_arrow(command_args[i]) && command_args[i])
			i = i + 1;
		new_word_start = i;
		while (command_args[i] && !is_arrow(command_args[i]))
			i += 1;
		result[word] = malloc(sizeof(char) * (i - new_word_start + 1));
		if (!result[word])
			return (ft_split_clear(result));
		counter = 0;
		while (new_word_start < i)
			result[word][counter++] = command_args[new_word_start++];
		result[word][counter] = '\0';
	}
	result[word] = NULL;
	return (result);	
}

int	single_pipe_(char **cmd_and_args, int fd_in, int fd_out, char **envp)
{
	// char **splitted;
	
	// splitted = split_by_arrows(one_cmd_and_args);
	int i = 0;
	while (cmd_and_args[i])
	{
		printf("splitted i %d single pipe %s\n", i, cmd_and_args[i]);
		i++;
	}
	//split << >> < >
	return(1);
}
// single_pipe_("ls -at", 3, 4, envp)
// single_pipe_("echo ", 3, 4, envp) use our echo (env, pwd, cd, )
// single_pipe_("ls -at > logs", 3, 4, envp)
// single_pipe_("ls -at >> logs", 3, 4, envp)
// single_pipe_(" < file << file2 ls -at", 3, 4, envp) 
// single_pipe_(" < file ls -at", 3, 4, envp)
// single_pipe_(" < file ls -at > file >> file", 3, 4, envp) 
// single_pipe_("ls -at>", 3, 4, envp) - fail
// single_pipe_("ls -at>>", 3, 4, envp) - fail
// single_pipe_(" < ls -at", 3, 4, envp) - fail(?)

int	single_pipe(char **splitted_input, int fd_in, int fd_out, char **envp)
{
	pid_t	pid;
	char	*path;
	char	**paths;

	pid = fork();
	if (!pid)
	{
		dups(fd_in, fd_out);
		if (fd_in == 3)
			close(4);
		if (fd_out == 4)
			close(3);
		paths = get_paths(envp);
		path = valid_path(paths, splitted_input[0]);
		if (!path)
			no_command(splitted_input, path, paths);
		execve(path, splitted_input, envp);
	}	
	waitpid(pid, NULL, 0);
	ft_split_clear(splitted_input);
	return (1);
}