/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:48:30 by sbritani          #+#    #+#             */
/*   Updated: 2023/01/09 16:51:21 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// void checkleaks()
// {
// 	system("leaks pipex");
// }
// atexit(&checkleaks);

void	exit_error(char *exit_message, t_pipex *pipex)
{
	perror(exit_message);
	clean_pipex(pipex);
	exit (1);
}

int	main(int argc, char **argv, char **env)
{
	int		i;
	t_pipex	pipex;

	if (argc < 5 || (!ft_strncmp(argv[1], "here_doc", 8) && argc < 6))
		return (1);
	i = pipex_init(&pipex, argv, argc, env);
	if (pipex.fd2 == -1)
		exit_error(argv[argc - 1], &pipex);
	if (pipex.fd == -1)
		exit_error(argv[1], &pipex);
	while (i <= pipex.number_of_pipes)
	{
		piping(&pipex, i, env);
		i++;
	}
	clean_pipex(&pipex);
	return (0);
}
