/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 15:45:27 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/07 12:55:36 by dhendzel         ###   ########.fr       */
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

void	read_from_to_shell(char *delimimter, int in_fd, int out_fd, int n_of_pipes)
{
	char	*buf;
	char	*pipes;

	pipes = repeat_line_n_times("pipe ", n_of_pipes);
	ft_putstr_fd(pipes, in_fd);
	ft_putstr_fd("heredoc> ", in_fd);
	buf = get_next_line(in_fd);
	while (buf && (ft_strncmp(buf, delimimter, ft_strlen(buf) - 1)
			|| ft_strlen(buf) == 1))
	{
		ft_putstr_fd(pipes, in_fd);
		ft_putstr_fd("heredoc> ", in_fd);
		ft_putstr_fd(buf, out_fd);
		free(buf);
		buf = get_next_line(in_fd);
	}
	free(pipes);
	free(buf);
}

char	*repeat_line_n_times(char *str, int n)
{
	char	*res;
	char	*mid_res;

	n = n - 1;
	if (n <= 0)
		return ("");
	res = ft_strdup(str);
	n--;
	while (n)
	{
		mid_res = ft_strjoin(res, str);
		free(res);
		res = ft_strdup(mid_res);
		free(mid_res);
		n--;
	}
	return (res);
}

void	close_truby(int **truby, int cur, int len)
{
	int	i;

	i = 0;
	if (cur == 0)
		close(truby[0][0]);
	else if (cur == len)
		close(truby[len - 1][1]);
	else
	{
		close(truby[cur - 1][1]);
		close(truby[cur][0]);
	}
	while (truby[i])
	{
		if (cur < i || cur > i + 1)
		{
			close(truby[i][0]);
			close(truby[i][1]);
		}
		i++;
	}
}

int	single_pipe_(char **cmd_and_args, int **truby, char **envp,pid_t	*pid, int num, int size)
{
	// char **splitted;
	char	**cmd;
	int		cmd_len;
	
	char	**paths;
	char	*path;
	int 	fd1;
	int 	fd2;

	int i = 0;
	cmd_len = 0;
	pid[num] = fork();
	if (!pid[num])
	{
		if (size)
		{
			if (num == 0)
				dup2(truby[0][1], STDOUT_FILENO);
			else if (num == size)
				dup2(truby[num - 1][0], STDIN_FILENO);
			else
			{
				dup2(truby[num - 1][0], STDIN_FILENO);
				dup2(truby[num][1], STDOUT_FILENO);
			}
			close_truby(truby, num, size);
		}
		cmd = malloc(sizeof(char *));
		cmd[0] = NULL;
		while (cmd_and_args[i])
		{
			if (strings_equal(cmd_and_args[i], "<"))
			{
				if(cmd_and_args[i+1])
				{
					fd1 = open(cmd_and_args[i+1], O_RDONLY);
					if(fd1 == -1)
					{
						perror("file not found");
						ft_split_clear(cmd);
						exit(127);
					}
					dup2(fd1, STDIN_FILENO);
					i++;
				}
				else
				{
					perror("file not found");
					ft_split_clear(cmd);
					exit(127);
				} 
			}
			// //need to implement pipes beforehand
			// // else if (strings_equal(cmd_and_args[i], "<<"))
			// // {
			// // 	if(cmd_and_args[i+1])
			// // 	{
			// // 		read_from_to_shell(cmd_and_args[i+1], STDIN_FILENO, STDIN_FILENO, 0);
					
			// // 		// dup2(fd2, STDOUT_FILENO);
			// // 		// dup standart input from cmd_and_args[i+1];
			// // 		i++;
			// // 	}
			// // 	else
			// // 	{
			// // 		perror("No delimiter specified");
			// // 		ft_split_clear(cmd);
			// // 		exit(127);
			// // 	} 
			// // }
			// 	// check if(cmd_and_args[i+1]), else error
			// 	// dup stdin to start reading from stdin until delim cmd_and_args[i+1]
			// 	// i++;
			else if (strings_equal(cmd_and_args[i], ">"))
			{
				if(cmd_and_args[i+1])
				{
					fd2 = open(cmd_and_args[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
					if(fd2 == -1)
					{
						perror("No output file specified");
						ft_split_clear(cmd);
						exit(127);
					}
					dup2(fd2, STDOUT_FILENO);
					i++;
				}
				else
				{
					perror("No output file specified");
					ft_split_clear(cmd);
					exit(127);
				} 
			}
			else if (strings_equal(cmd_and_args[i+1], ">>"))
			{
				if(cmd_and_args[i+1])
				{
					fd2 = open(cmd_and_args[i+1], O_RDWR | O_CREAT | O_APPEND, 0644);
					if(fd2 == -1)
					{
						perror("No output file specified");
						ft_split_clear(cmd);
						exit(127);
					}
					dup2(fd2, STDOUT_FILENO);
					i++;
				}
				else
				{
					perror("No output file specified");
					ft_split_clear(cmd);
					exit(127);
				} 
			}
			else
			{
					cmd = add_string_to_string_arr(cmd_and_args[i], cmd, cmd_len);
					cmd_len++;
			}
			i++;
		}
			paths = get_paths(envp);
			path = valid_path(paths, cmd[0]);
			if (!path)
				no_command(cmd, path, paths);
			execve(path, cmd, envp);
	}
	return(1);
}
// int	single_pipe_(char **cmd_and_args, int fd_in, int fd_out, char **envp, int **pip,pid_t	*pid, int num, int **pip2)
// {
// 	// char **splitted;
// 	char	**cmd;
// 	int		cmd_len;
	
// 	char	**paths;
// 	char	*path;
// 	int 	fd1;
// 	int 	fd2;

// 	// splitted = split_by_arrows(one_cmd_and_args);
// 	int i = 0;
// 	cmd_len = 0;

// 			printf("starting child %d\n", num);
// 	pid[num] = fork();
// 	if (!pid[num])
// 	{
// 		//if pipe
// 		//dup input and output
// 		if (!fd_in)
// 		{
// 			dup2((*pip)[1], STDOUT_FILENO);
// 			close((*pip)[0]);
// 			close((*pip2)[0]);
// 			close((*pip2)[1]);
// 		}
// 		else if (fd_out == 1)
// 		{
// 			dup2((*pip2)[0], STDIN_FILENO);
// 			close ((*pip2)[1]);
// 			close ((*pip)[1]);
// 			close ((*pip)[0]);
// 		}
// 		else
// 		{
// 			dup2((*pip)[0], STDIN_FILENO);
// 			dup2((*pip2)[1], STDOUT_FILENO);
// 			close ((*pip)[1]);	
// 			close ((*pip2)[0]);	
// 		}
// 		// dup2(fd_out, STDOUT_FILENO);
// 		cmd = malloc(sizeof(char *));
// 		cmd[0] = NULL;
// 		while (cmd_and_args[i])
// 		{
// 			// 		ft_putstr_fd("ebal1\n", 2);
// 			if (strings_equal(cmd_and_args[i], "<"))
// 			{
// 				if(cmd_and_args[i+1])
// 				{
// 					fd1 = open(cmd_and_args[i+1], O_RDONLY);
// 					if(fd1 == -1)
// 					{
// 						perror("file not found");
// 						ft_split_clear(cmd);
// 						exit(127);
// 					}
// 					dup2(fd1, STDIN_FILENO);
// 					// dup standart input from cmd_and_args[i+1];
// 					i++;
// 				}
// 				else
// 				{
// 					perror("file not found");
// 					ft_split_clear(cmd);
// 					exit(127);
// 				} 
// 			}
// 			// //need to implement pipes beforehand
// 			// // else if (strings_equal(cmd_and_args[i], "<<"))
// 			// // {
// 			// // 	if(cmd_and_args[i+1])
// 			// // 	{
// 			// // 		read_from_to_shell(cmd_and_args[i+1], STDIN_FILENO, STDIN_FILENO, 0);
					
// 			// // 		// dup2(fd2, STDOUT_FILENO);
// 			// // 		// dup standart input from cmd_and_args[i+1];
// 			// // 		i++;
// 			// // 	}
// 			// // 	else
// 			// // 	{
// 			// // 		perror("No delimiter specified");
// 			// // 		ft_split_clear(cmd);
// 			// // 		exit(127);
// 			// // 	} 
// 			// // }
// 			// 	// check if(cmd_and_args[i+1]), else error
// 			// 	// dup stdin to start reading from stdin until delim cmd_and_args[i+1]
// 			// 	// i++;
// 			else if (strings_equal(cmd_and_args[i], ">"))
// 			{
// 				if(cmd_and_args[i+1])
// 				{
// 					fd2 = open(cmd_and_args[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
// 					if(fd2 == -1)
// 					{
// 						perror("No output file specified");
// 						ft_split_clear(cmd);
// 						exit(127);
// 					}
// 					dup2(fd2, STDOUT_FILENO);
// 					// dup standart input from cmd_and_args[i+1];
// 					i++;
// 				}
// 				else
// 				{
// 					perror("No output file specified");
// 					ft_split_clear(cmd);
// 					exit(127);
// 				} 
// 			}
// 			// 	// check if(cmd_and_args[i+1]), else error
// 			// 	// dup stdout to file cmd_and_args[i+1] in new mode
// 			// 	// i++;
// 			else if (strings_equal(cmd_and_args[i+1], ">>"))
// 			{
// 				if(cmd_and_args[i+1])
// 				{
// 					fd2 = open(cmd_and_args[i+1], O_RDWR | O_CREAT | O_APPEND, 0644);
// 					if(fd2 == -1)
// 					{
// 						perror("No output file specified");
// 						ft_split_clear(cmd);
// 						exit(127);
// 					}
// 					dup2(fd2, STDOUT_FILENO);
// 					// dup standart input from cmd_and_args[i+1];
// 					i++;
// 				}
// 				else
// 				{
// 					perror("No output file specified");
// 					ft_split_clear(cmd);
// 					exit(127);
// 				} 
// 			}
// 				// check if(cmd_and_args[i+1]), else error
// 				// dup stdout to file cmd_and_args[i+1]
// 				// i++;
// 			else
// 			{
// 					cmd = add_string_to_string_arr(cmd_and_args[i], cmd, cmd_len);
// 					cmd_len++;
// 			}
// 			i++;
// 		}
// 			paths = get_paths(envp);
// 			path = valid_path(paths, cmd[0]);
// 			if (!path)
// 				no_command(cmd, path, paths);
// 			execve(path, cmd, envp);
// 	}
// 	// i = 0;
// 	// while (cmd[i])
// 	// {
// 	// 	printf("splitted i %d single pipe %s\n", i, cmd[i]);
// 	// 	i++;
// 	// }
// 	// ft_split_clear(cmd);
// 	return(1);
// }
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