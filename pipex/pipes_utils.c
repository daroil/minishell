/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 03:20:42 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/23 13:58:26 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	array_len(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

void	interrupt_input_doc(int signal)
{
	if (signal || !signal)
	{
		printf("\r");
	}
	exit(127);
}

void	clean_exit(char **cmd)
{
	ft_split_clear(cmd);
	exit(1);
}
