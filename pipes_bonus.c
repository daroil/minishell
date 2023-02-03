/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:49:33 by sbritani          #+#    #+#             */
/*   Updated: 2023/01/09 16:51:10 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	pipex_init(t_pipex *pipex, char **argv, int argc, char **env)
{
	char	**args;

	pipex->cmds = transform_args_to_cmds(argv);
	pipex->here_doc = !ft_strncmp(pipex->cmds[0], "here_doc", 8);
	pipex->number_of_children = argc - 3;
	pipex->number_of_pipes = pipex->number_of_children - 1;
	pipex->truby = make_truby(pipex->number_of_pipes);
	pipex->pids = make_pids(pipex->number_of_children);
	pipex->fd = 1;
	if (!pipex->here_doc)
		pipex->fd = open(argv[1], O_RDONLY);
	pipex->paths = get_paths(env);
	if (!pipex->here_doc)
		pipex->fd2 = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else
	{
		pipex->fd2 = open(argv[argc - 1], O_RDWR | O_CREAT | O_APPEND, 0644);
		args = ft_split(pipex->cmds[0], " ");
		read_from_to(args[1], STDIN_FILENO, pipex->truby[0][1],
			pipex->number_of_pipes);
		ft_split_clear(args);
		return (1);
	}
	return (0);
}

void	clean_pipex(t_pipex *pipex)
{
	plumber(pipex->truby);
	if (pipex->here_doc)
		free(pipex->cmds[0]);
	free(pipex->cmds);
	waitress(pipex->pids, pipex->number_of_children);
	close(pipex->fd);
	close(pipex->fd2);
	ft_split_clear(pipex->paths);
	free(pipex->pids);
}

void	piping(t_pipex *pipex, int i, char **env)
{
	char	**args;
	char	*path;

	pipex->pids[i] = fork();
	if (!pipex->pids[i])
	{
		if (i == 0)
			dups(pipex->fd, pipex->truby[0][1]);
		else if (i == pipex->number_of_children - 1)
			dups(pipex->truby[i - 1][0], pipex->fd2);
		else
			dups(pipex->truby[i - 1][0], pipex->truby[i][1]);
		close_truby(pipex->truby, i, pipex->number_of_pipes);
		args = ft_split(pipex->cmds[i], " ");
		path = valid_path(pipex->paths, args[0]);
		if (!path)
			error(pipex, args, path);
		execve(path, args, env);
	}	
}

pid_t	*make_pids(int n)
{
	pid_t	*res;

	res = malloc(sizeof(pid_t) * n);
	return (res);
}

void	waitress(pid_t *pids, int len)
{
	while (len)
		waitpid(pids[--len], NULL, 0);
}
