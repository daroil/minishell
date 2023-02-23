/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_in_out_put.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 03:19:17 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/23 13:54:11 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	read_from_to_shell(char *delimimter, int in_fd, int out_fd)
{
	char	*buf;

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
