/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 15:45:27 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/20 13:33:22 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	dups(int fd1, int fd2)
// {
// 	dup2(fd1, STDIN_FILENO);
// 	dup2(fd2, STDOUT_FILENO);
// }

void	read_from_to_shell(char *delimimter, int in_fd, int out_fd)
{
	char	*buf;
	char	*pipes;

	ft_putstr_fd("> ", in_fd);
	buf = get_next_line(in_fd);
	while (buf && (ft_strncmp(buf, delimimter, ft_strlen(buf) - 1)
			|| ft_strlen(buf) == 1))
	{
		ft_putstr_fd("> ", in_fd);
		ft_putstr_fd(buf, out_fd);
		free(buf);
		buf = get_next_line(in_fd);
	}
	free(buf);
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

void	interrupt_input_doc(int sig)
{
	printf("\r");
	exit(127);
}

int	array_len(char **array)
{
	int i;

	i = 0;
	while(array[i])
		i++;
	return(i);
}


int	single_pipe(char **cmd_and_args, t_pipex pipex, char **envp, t_settings *settings)
{
	char	**cmd;
	int		cmd_len;
	char	**paths;
	char	*path;
	int 	fd1;
	int 	fd2;

	pid_t *pid = pipex.pid; 
	int num = pipex.i;
	int size = pipex.number_of_pipes;
	int **truby = pipex.truby;
	int redirect_input;
	int redirect_output;

	redirect_input = 0;
	redirect_output = 0;
	int i = 0;
	cmd_len = 0;
	pid[num] = fork();
	if (!pid[num])
	{
		
		//allocating memory for command
		cmd = malloc(sizeof(char *));
		cmd[0] = NULL;
		while (cmd_and_args[i])
		{
			//check if infile should be duped
			if (strings_equal(cmd_and_args[i], "<"))
			{
				redirect_input = 1;
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
			//check if here_doc
			else if (strings_equal(cmd_and_args[i], "<<"))
			{
				redirect_input = 1;
				if(cmd_and_args[i+1])
				{
					int	heredoc_pipe[2];
					char *buf;
					
					pipe(heredoc_pipe);
					signal(SIGINT, interrupt_input_doc);
					read_from_to_shell(cmd_and_args[i+1], STDIN_FILENO, heredoc_pipe[1]);
					close(heredoc_pipe[1]);
					if (array_len(cmd_and_args) == 2)
					{
						buf = get_next_line(heredoc_pipe[0]);
						while (buf)
						{
							ft_putstr_fd(buf, 1);
							free(buf);
							buf = get_next_line(heredoc_pipe[0]);
						}
						free(buf);
						close(heredoc_pipe[0]);	
					}
					else
						dup2(heredoc_pipe[0], STDIN_FILENO);
					i++;
				}
				else
				{
					perror("No delimiter specified");
					ft_split_clear(cmd);
					exit(127);
				} 
			}
			//check if outfile is changed
			else if (strings_equal(cmd_and_args[i], ">"))
			{
				redirect_output = 1;
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
			//check if outfile is in append mode
			else if (strings_equal(cmd_and_args[i], ">>"))
			{
				redirect_output = 1;
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
			//adding to command line
			else
			{
					cmd = add_string_to_string_arr(cmd_and_args[i], cmd, cmd_len);
					cmd_len++;
			}
			i++;
		}
		// doing pipes if needed
		if (size)
		{
			if (num == 0 && !redirect_output)
				dup2(truby[0][1], STDOUT_FILENO);
			else if (num == size && !redirect_input)
				dup2(truby[num - 1][0], STDIN_FILENO);
			else
			{
				if (!redirect_input)
					dup2(truby[num - 1][0], STDIN_FILENO);
				if (num != size && !redirect_output)
					dup2(truby[num][1], STDOUT_FILENO);
			}
			close_truby(truby, num, size);
		}
		//our commands
		if (strings_equal(cmd[0], "exit"))
		{
			ft_split_clear(cmd);
			exit(0);
		}
		if (!cmd[0])
		{
			ft_split_clear(cmd);
			exit(1);
		}
		if (strings_equal(cmd[0], "echo\0"))
		{
			echo(cmd + 1);
			ft_split_clear(cmd);
			exit(1);
		}
		if (strings_equal(cmd[0], "pwd\0"))
		{
			pwd(cmd);
			ft_split_clear(cmd);
			exit(1);
		}
		if (strings_equal(cmd[0], "cd\0"))
		{
			cd(cmd, settings);
			ft_split_clear(cmd);
			exit (1);
		}
		if (strings_equal(cmd[0], "unset\0"))
		{
			unset(cmd, settings);
			ft_split_clear(cmd);
			exit(1);
		}
		if (strings_equal(cmd[0], "export\0"))
		{
			export(cmd, settings);
			ft_split_clear(cmd);
			exit(1);
		}
		if (ft_strchr(cmd[0], '='))
		{
			deal_with_equal_sign(cmd, settings);
			ft_split_clear(cmd);
			exit(1);
		}
		
		//check if command exists and executing it
		paths = get_paths(envp);
		path = valid_path(paths, cmd[0]);
		if (!path)
			no_command(cmd, path, paths);
		execve(path, cmd, envp);
	}
	return(1);
}
