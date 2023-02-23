/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 03:37:41 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/23 04:02:22 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*cur_dir(void)
{
	char	*res;

	res = malloc(sizeof(char) * 1000);
	if (!getcwd(res, 1000))
	{
		free(res);
		return (NULL);
	}
	return (res);
}

int	pwd(char **splitted_input)
{
	char	*to_print;

	if (splitted_input[1])
		printf("pwd: too many arguments\n");
	else
	{
		to_print = cur_dir();
		printf("%s\n", to_print);
		free(to_print);
	}
	return (0);
}

int	cd_home(char **splitted_input, t_settings *settings)
{
	char	*temp;
	char	*other_temp;

	temp = settings->last_working_directory;
	settings->last_working_directory = cur_dir();
	chdir("/");
	dict_add(settings->exported_env, "OLDPWD\0",
		settings->last_working_directory);
	other_temp = cur_dir();
	dict_add(settings->exported_env, "PWD\0", other_temp);
	free(other_temp);
	free(temp);
	return (0);
}

int	cd_minus(char **splitted_input, t_settings *settings)
{
	char	*temp;
	char	*other_temp;

	temp = settings->last_working_directory;
	settings->last_working_directory = cur_dir();
	chdir(temp);
	dict_add(settings->exported_env, "OLDPWD\0",
		settings->last_working_directory);
	other_temp = cur_dir();
	dict_add(settings->exported_env, "PWD\0", other_temp);
	free(other_temp);
	free(temp);
	return (0);
}

int	cd(char **splitted_input, t_settings *settings)
{
	char	*temp;
	char	*other_temp;

	if (!splitted_input[1])
		return (cd_home(splitted_input, settings));
	if (strings_equal(splitted_input[1], "-\0"))
		return (cd_minus(splitted_input, settings));
	if (!access(splitted_input[1], X_OK))
	{
		free(settings->last_working_directory);
		settings->last_working_directory = cur_dir();
		chdir(splitted_input[1]);
		dict_add(settings->exported_env, "OLDPWD\0",
			settings->last_working_directory);
		other_temp = cur_dir();
		dict_add(settings->exported_env, "PWD\0", other_temp);
		free(other_temp);
		return (0);
	}
	printf("cd: no such file or directory: %s\n", splitted_input[1]);
	return (1);
}
