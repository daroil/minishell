/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:50:31 by sbritani          #+#    #+#             */
/*   Updated: 2023/01/09 16:51:31 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include "libft/libft.h"
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <fcntl.h>

typedef struct s_pipex
{
	char	**paths;
	int		**truby;
	int		fd;
	int		fd2;
	int		number_of_children;
	int		number_of_pipes;
	pid_t	*pids;
	int		here_doc;
	char	**cmds;
}	t_pipex;

void	read_from_to(char *delimimter, int in_fd, int out_fd, int n_of_pipes);
char	*repeat_line_n_times(char *str, int n);
char	*join_three_lines(char *str1, char *str2, char *str3);
char	**get_paths(char **env);
char	*valid_path(char **paths, char *filename);
int		**make_truby(int n);
void	close_truby(int **truby, int cur, int len);
void	plumber(int **truby);
pid_t	*make_pids(int n);
void	waitress(pid_t *pids, int len);
char	**transform_args_to_cmds(char **argv);
void	dups(int fd1, int fd2);
int		pipex_init(t_pipex *pipex, char **argv, int argc, char **env);
void	clean_pipex(t_pipex *pipex);
void	error(t_pipex *pipex, char **args, char *path);
void	piping(t_pipex *pipex, int i, char **env);

#endif
