/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 03:27:24 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/23 03:28:11 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*valid_path(char **paths, char *filename)
{
	char	*full_path;
	char	*path_with_slash;
	int		i;

	i = -1;
	if (!paths)
		return (NULL);
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

int	check_path(char **envp, char *splitted_input)
{
	char	**paths;
	char	*path;

	paths = get_paths(envp);
	path = valid_path(paths, splitted_input);
	ft_split_clear(paths);
	if (!path)
	{
		free(path);
		return (0);
	}
	free(path);
	return (1);
}

int	string_in_array_of_strings(char *string, char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if (strings_equal(array[i], string))
			return (1);
		i++;
	}
	return (0);
}

void	no_command(char **splitted_input, char *path, char **paths)
{
	ft_putstr_fd(splitted_input[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror("command not found");
	ft_split_clear(paths);
	free(path);
	ft_split_clear(splitted_input);
	exit (127);
}
