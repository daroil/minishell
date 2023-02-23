/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 21:26:30 by sbritani          #+#    #+#             */
/*   Updated: 2023/02/23 03:44:27 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_next_arg_return	*init_next_arg(void)
{
	t_next_arg_return	*res;

	res = malloc(sizeof(t_next_arg_return));
	res->arg = NULL;
	res->last_index = 0;
	return (res);
}

void	free_next_arg_return(t_next_arg_return *next_arg)
{
	if (next_arg)
	{
		if (next_arg->arg)
			free(next_arg->arg);
		free(next_arg);
	}
}

void	clear_splits(char **splitted_input, char ***resplitted_input)
{
	ft_split_clear(splitted_input);
	free_resplitted(resplitted_input);
}
