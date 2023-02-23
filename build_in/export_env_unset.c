/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_env_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 03:39:00 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/23 14:33:17 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env(char **splitted_input, t_settings *settings)
{
	int	i;

	i = 0;
	if (splitted_input[1])
	{
		printf("sorry in current version env doesn't take any  ameters\n");
		return (127);
	}
	while (settings->exported_env->keys[i])
	{
		if (settings->exported_env->values[i])
			printf("%s=%s\n", settings->exported_env->keys[i],
				settings->exported_env->values[i]);
		i++;
	}
	return (0);
}

int	unset(char **splitted_input, t_settings *settings)
{
	int	i;

	i = 1;
	while (splitted_input[i])
	{
		dict_delete(settings->env, splitted_input[i]);
		dict_delete(settings->exported_env, splitted_input[i]);
		i++;
	}
	return (0);
}

char	**split_for_equal_sign(char *str)
{
	char	**res;
	int		i;

	i = 0;
	res = malloc(sizeof(char *) * 3);
	res[2] = NULL;
	while (str[i] && str[i] != '=')
		i++;
	res[0] = str_copy(str, i);
	res[1] = str_copy(str + i + 1, -1);
	if (!res[1][0])
	{
		free(res[1]);
		res[1] = NULL;
	}
	return (res);
}

int	export(char **splitted_input, t_settings *settings)
{
	int		i;
	char	**temp;

	i = 1;
	while (splitted_input[i])
	{
		temp = split_for_equal_sign(splitted_input[i]);
		if (temp[1] && temp[1][0] != '\0')
		{
			dict_add(settings->env, temp[0], temp[1]);
			dict_add(settings->exported_env, temp[0], temp[1]);
		}
		else if (dict_get(settings->env, temp[0], NULL))
		{
			dict_add(settings->exported_env, temp[0],
				dict_get(settings->env, temp[0], NULL));
		}
		ft_split_clear(temp);
		i++;
	}
	return (0);
}

int	deal_with_equal_sign(char **splitted_input, t_settings *settings)
{
	int		i;
	char	**temp;

	i = 0;
	while (splitted_input[i])
	{
		temp = split_for_equal_sign(splitted_input[i]);
		if (temp[1])
			dict_add(settings->env, temp[0], temp[1]);
		else
		{
			ft_split_clear(temp);
			return (127);
		}
		ft_split_clear(temp);
		i++;
	}
	return (0);
}
