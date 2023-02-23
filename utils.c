/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 18:43:10 by sbritani          #+#    #+#             */
/*   Updated: 2023/02/23 03:41:05 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_two_strings(char *str1, char *str2)
{
	free(str1);
	free(str2);
}

char	*str_copy(char *str, int n)
{
	char	*res;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	if (n < 0)
	{
		while (str[i++])
			;
		res = malloc(sizeof(char) * i);
		i = -1;
		while (str[++i])
			res[i] = str[i];
		res[i] = '\0';
	}
	else
	{
		res = malloc(sizeof(char) * (n + 1));
		i = -1;
		while (str[++i] && i < n)
			res[i] = str[i];
		res[i] = '\0';
	}
	return (res);
}

char	**add_string_to_string_arr(char *str, char **arr, int n)
{
	char	**res;
	int		i;

	i = 0;
	res = malloc(sizeof(char *) * (n + 2));
	while (i < n)
	{
		res[i] = arr[i];
		i++;
	}
	res[i] = str_copy(str, -1);
	res[i + 1] = NULL;
	free(arr);
	return (res);
}

char	**remove_string_from_str_arr(char **arr, int n)
{
	char	**res;
	int		i;

	i = 0;
	while (arr[i++])
		;
	res = malloc (sizeof(char *) * (i - 1));
	i = -1;
	while (arr[++i])
	{
		if (i < n)
			res[i] = arr[i];
		else if (i > n)
			res[i - 1] = arr[i];
	}
	res[i - 1] = NULL;
	free(arr[n]);
	free(arr);
	return (res);
}

void	free_last_cmd(t_settings *settings)
{
	free(settings->last_cmd);
	settings->last_cmd = NULL;
}
