/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 21:26:30 by sbritani          #+#    #+#             */
/*   Updated: 2023/01/24 12:42:58 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_next_arg_return	*init_next_arg(void)
{
	t_next_arg_return *res;

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