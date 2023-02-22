/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 15:45:27 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/22 23:18:56 by dhendzel         ###   ########.fr       */
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
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

int	infile_change(t_pipex *pipex, char **cmd, int i, char **cmd_and_args)
{
	int	fd;

	pipex->redirect_input = 1;
	if (cmd_and_args[i + 1])
	{
		fd = open(cmd_and_args[i + 1], O_RDONLY);
		if (fd == -1)
		{
			perror("file not found");
			ft_split_clear(cmd);
			exit(127);
		}
		dup2(fd, STDIN_FILENO);
		return (1);
	}
	else
	{
		perror("file not found");
		ft_split_clear(cmd);
		exit(127);
	}
}

int	outfile_change(t_pipex *pipex, char **cmd, int i, char **cmd_and_args)
{
	int	fd;

	pipex->redirect_output = 1;
	if (cmd_and_args[i + 1])
	{
		fd = open(cmd_and_args[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd == -1)
		{
			perror("No output file specified");
			ft_split_clear(cmd);
			exit(127);
		}
		dup2(fd, STDOUT_FILENO);
		return (1);
	}
	else
	{
		perror("No output file specified");
		ft_split_clear(cmd);
		exit(127);
	}
}

int	outfile_change_append(t_pipex *pipex, char **cmd,
	int i, char **cmd_and_args)
{
	int	fd;

	pipex->redirect_output = 1;
	if (cmd_and_args[i + 1])
	{
		fd = open(cmd_and_args[i + 1], O_RDWR | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			perror("No output file specified");
			ft_split_clear(cmd);
			exit(127);
		}
		dup2(fd, STDOUT_FILENO);
		return (1);
	}
	else
	{
		perror("No output file specified");
		ft_split_clear(cmd);
		exit(127);
	}
}

int	infile_heredoc(t_pipex *pipex, char **cmd, int i, char **cmd_and_args)
{
	int	heredoc_pipe[2];

	pipex->redirect_input = 1;
	if (cmd_and_args[i + 1])
	{
		signal(SIGINT, interrupt_input_doc);
		pipe(heredoc_pipe);
		read_from_to_shell(cmd_and_args[i + 1], STDIN_FILENO, heredoc_pipe[1]);
		close(heredoc_pipe[1]);
		dup2(heredoc_pipe[0], STDIN_FILENO);
		return (1);
	}
	else
	{
		perror("No delimiter specified");
		ft_split_clear(cmd);
		exit(127);
	}
}

void	duping(int size, int num, t_pipex *pipex)
{
	if (num == 0 && !pipex->redirect_output)
		dup2(pipex->truby[0][1], STDOUT_FILENO);
	else if (num == size && !pipex->redirect_input)
		dup2(pipex->truby[num - 1][0], STDIN_FILENO);
	else
	{
		if (!pipex->redirect_input)
			dup2(pipex->truby[num - 1][0], STDIN_FILENO);
		if (num != size && !pipex->redirect_output)
			dup2(pipex->truby[num][1], STDOUT_FILENO);
	}
	close_truby(pipex->truby, num, size);
}

int	basic_commands_pipe(char **cmd, t_settings *settings)
{
	if (strings_equal(cmd[0], "exit"))
		return (1);
	if (!cmd[0])
		return (1);
	if (strings_equal(cmd[0], "echo\0"))
		return (echo(cmd + 1), 1);
	if (strings_equal(cmd[0], "pwd\0"))
		return (pwd(cmd), 1);
	if (strings_equal(cmd[0], "cd\0"))
		return (cd(cmd, settings), 1);
	if (strings_equal(cmd[0], "unset\0"))
		return (unset(cmd, settings), 1);
	if (strings_equal(cmd[0], "export\0"))
		return (export(cmd, settings), 1);
	if (ft_strchr(cmd[0], '='))
		return (deal_with_equal_sign(cmd, settings), 1);
	return (0);
}

void	clean_exit(char **cmd)
{
	ft_split_clear(cmd);
	exit(1);
}

char	**change_in_out_put_create_cmd(char **cmd_and_args,
	t_pipex *pipex, char **cmd)
{
	int	i;
	int	cmd_len;

	i = 0;
	cmd_len = 0;
	cmd = malloc(sizeof(char *));
	cmd[0] = NULL;
	while (cmd_and_args[i])
	{
		if (strings_equal(cmd_and_args[i], "<"))
			i += infile_change(pipex, cmd, i, cmd_and_args);
		else if (strings_equal(cmd_and_args[i], "<<"))
			i += infile_heredoc(pipex, cmd, i, cmd_and_args);
		else if (strings_equal(cmd_and_args[i], ">"))
			i += outfile_change(pipex, cmd, i, cmd_and_args);
		else if (strings_equal(cmd_and_args[i], ">>"))
			i += outfile_change_append(pipex, cmd, i, cmd_and_args);
		else
		{
			cmd = add_string_to_string_arr(cmd_and_args[i], cmd, cmd_len);
			cmd_len++;
		}
		i++;
	}
	return (cmd);
}

int	single_pipe(char **cmd_and_args, t_pipex pipex,
	char **envp, t_settings *settings)
{
	char	**cmd;
	char	**paths;
	char	*path;

	pipex.pid[pipex.i] = fork();
	if (!pipex.pid[pipex.i])
	{
		cmd = change_in_out_put_create_cmd(cmd_and_args, &pipex, cmd);
		if (pipex.number_of_pipes)
			duping(pipex.number_of_pipes, pipex.i, &pipex);
		if (basic_commands_pipe(cmd, settings))
			clean_exit(cmd);
		paths = get_paths(envp);
		path = valid_path(paths, cmd[0]);
		if (!path)
			no_command(cmd, path, paths);
		execve(path, cmd, envp);
	}
	return (1);
}
