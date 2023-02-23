/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 15:18:22 by sbritani          #+#    #+#             */
/*   Updated: 2023/02/23 15:28:58 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	export_print(t_settings *settings)
{
	int	i;

	i = 0;
	while (i < settings->exported_env->len)
	{
		printf("declare -x %s=%s\n", settings->exported_env->keys[i],
			settings->exported_env->values[i]);
		i++;
	}
	return (0);
}
