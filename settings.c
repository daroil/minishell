/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:50:23 by sbritani          #+#    #+#             */
/*   Updated: 2023/02/16 13:36:27 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_env(char **env, t_settings *settings)
{
	char	**temp;
	int		i;

	i = 0;
	while (env[i])
	{
		temp = ft_split(env[i], "=");
		
		dict_add(settings->env, temp[0], temp[1]);
		dict_add(settings->exported_env, temp[0], temp[1]);
		ft_split_clear(temp);
		i++;
	}
}

t_settings	*create_setttings(char **env)
{
	t_settings *res;

	res = malloc(sizeof(t_settings));
	res->env = init_dict();
	res->exported_env = init_dict();
	res->last_cmd = NULL;
	res->pipex = NULL;
	parse_env(env, res);
	return (res);
}

void	clear_settings(t_settings *settings)
{
	free(settings->last_working_directory);
	free(settings->last_cmd);
	free_dict(settings->env);
	free_dict(settings->exported_env);
	free(settings);
}
