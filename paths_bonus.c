/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:49:22 by sbritani          #+#    #+#             */
/*   Updated: 2023/01/09 16:51:16 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*join_three_lines(char *str1, char *str2, char *str3)
{
	char	*res;
	char	*mid_res;

	mid_res = ft_strjoin(str1, str2);
	res = ft_strjoin(mid_res, str3);
	free(mid_res);
	return (res);
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

char	**transform_args_to_cmds(char **argv)
{
	char	**res;
	int		i;

	i = 0;
	while (argv[i++])
		;
	i = i - 3;
	i = i - (!ft_strncmp(argv[1], "here_doc\0", 9));
	res = malloc(sizeof(char *) * i);
	i = 0;
	if ((!ft_strncmp(argv[1], "here_doc\0", 9)))
	{
		res[0] = join_three_lines(argv[1], " ", argv[2]);
		i++;
	}
	while (argv[i + 3])
	{
		res[i] = argv[i + 2];
		i++;
	}
	res[i] = NULL;
	return (res);
}
