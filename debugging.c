/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 18:46:07 by sbritani          #+#    #+#             */
/*   Updated: 2023/02/07 11:49:10 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_dict(t_dict *dict)
{
	int	i;
	i = 0;
	while (dict->keys[i])
	{
		printf("%s=%s\n", dict->keys[i],dict->values[i]);
		i++;
	}
}

void	print_splitted(char **splitted)
{
	int	i;

	i = 0;
	while (splitted[i])
	{
		printf("{%s}\n", splitted[i]);
		i++;
	}
}

void	print_resplitted(char ***resplitted)
{
	int i;
	if (!resplitted)
		{printf("%s\n", NULL);
		return ;
		}
	i = 0;
	while (resplitted[i])
	{
		printf("%d resplitted array :\n", i);
		print_splitted(resplitted[i]);
		i++;
	}
	printf("finished printing resplitted\n");
}