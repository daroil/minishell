/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 03:20:42 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/23 03:22:13 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	array_len(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

void	interrupt_input_doc(int sig)
{
	printf("\r");
	exit(127);
}

void	clean_exit(char **cmd)
{
	ft_split_clear(cmd);
	exit(1);
}
