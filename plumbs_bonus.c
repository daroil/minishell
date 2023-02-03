/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plumbs_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:49:45 by sbritani          #+#    #+#             */
/*   Updated: 2023/01/09 16:51:03 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	**make_truby(int n)
{
	int	**res;
	int	i;

	res = malloc(sizeof(int *) * (n + 1));
	i = 0;
	while (i < n)
	{
		res[i] = malloc(sizeof(int) * 2);
		pipe(res[i]);
		i++;
	}
	res[i] = NULL;
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

void	plumber(int **truby)
{
	int	i;

	i = 0;
	while (truby[i])
	{
		close(truby[i][1]);
		close(truby[i][0]);
		free(truby[i]);
		i++;
	}
	free(truby);
}

void	dups(int fd1, int fd2)
{
	dup2(fd1, STDIN_FILENO);
	dup2(fd2, STDOUT_FILENO);
}

void	error(t_pipex *pipex, char **args, char *path)
{
	ft_putstr_fd(args[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror("command not found");
	ft_split_clear(args);
	free(path);
	clean_pipex(pipex);
	exit (127);
}
