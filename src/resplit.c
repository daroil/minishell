/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resplit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 03:09:17 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/23 04:05:22 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	***resplit(char **splitted)
{
	char	***res;
	int		prev_start;
	int		i;

	prev_start = 0;
	res = NULL;
	i = prev_start;
	while (splitted[i])
	{
		if (!strings_equal(splitted[i], "|\0"))
			i++;
		else
		{
			res = add_string_array_to_array_of_string_arrays(res,
					copy_str_array(splitted + prev_start, i - prev_start));
			prev_start = i + 1;
			i++;
		}
	}
	res = add_string_array_to_array_of_string_arrays(res,
			copy_str_array(splitted + prev_start, i - prev_start));
	i++;
	res = add_string_array_to_array_of_string_arrays(res, NULL);
	return (res);
}

int	count_resplitted(char ***resplitted)
{
	int	i;

	i = 0;
	if (!resplitted)
		return (0);
	while (resplitted[i])
		i++;
	return (i);
}

void	free_resplitted(char ***resplitted)
{
	int	i;

	if (!resplitted)
		return ;
	i = 0;
	while (resplitted [i])
	{
		ft_split_clear(resplitted[i]);
		i++;
	}
	free(resplitted);
}

char	***add_string_array_to_array_of_string_arrays(char ***old,
			char **to_add)
{
	char	***res;
	int		i;

	if (!old)
	{
		res = malloc(sizeof(char **) * 2);
		res[0] = to_add;
		res[1] = NULL;
		return (res);
	}
	i = 0;
	while (old[i])
		i++;
	res = malloc(sizeof(char **) * (i + 2));
	i = 0;
	while (old[i])
	{
		res[i] = old[i];
		i++;
	}
	res[i] = to_add;
	res[i + 1] = NULL;
	free(old);
	return (res);
}

char	**copy_str_array(char **array, int n)
{
	char	**res;
	int		i;

	if (!array)
		return (NULL);
	i = 0;
	while (array[i] && (i < n || n < 0))
		i++;
	res = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (array[i] && (i < n || n < 0))
	{
		res[i] = str_copy(array[i], -1);
		i++;
	}
	res[i] = NULL;
	return (res);
}
