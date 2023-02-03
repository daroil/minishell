/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 15:55:13 by sbritani          #+#    #+#             */
/*   Updated: 2023/01/23 22:35:33 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(char **args)
{
	int	i;
	i = 0;
	if (!args || !args[0])
	{
		printf("\n");
		return (0);
	}
	if(strings_equal(args[0], "-n\0"))
		i++;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if(!strings_equal(args[0], "-n\0"))
		printf("\n");
	return (0);
	
}