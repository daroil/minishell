/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 19:18:48 by sbritani          #+#    #+#             */
/*   Updated: 2023/02/23 13:44:28 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_next_arg_return	*deal_with_dollar(char *input, t_settings *settings)
{
	t_next_arg_return	*res;
	char				*mid_line;
	int					i;

	res = init_next_arg();
	i = 0;
	if (*input == '?')
	{
		res->arg = ft_itoa(settings->last_exit_status);
		res->last_index++;
		return (res);
	}
	while (input[i] && !is_bash_special_char(input[i]) && input[i] != '\t')
		i++;
	mid_line = str_copy(input, i);
	res->arg = str_copy(dict_get(settings->env, mid_line, NULL), -1);
	free(mid_line);
	res->last_index = i;
	return (res);
}

t_next_arg_return	*deal_with_double_quotes(char *input, t_settings *settings)
{
	int					i;
	t_next_arg_return	*res;

	i = 0;
	res = init_next_arg();
	res->last_index = i;
	while (input[i] && input[i] != '"')
	{
		while (input[i] && input[i] != '"' && input[i] != '$')
			i++;
		res->arg = ft_str_join_free_both(res->arg,
				str_copy(input + res->last_index, i - res->last_index));
		res->last_index = i;
		if (input[i] == '$')
			i += for_spec_char_return_index(input, res, settings, i);
		if (input[i] == '"')
			return (res);
	}
	if (!input[i])
		return (no_input(res, i));
	res->last_index = i;
	return (res);
}

t_next_arg_return	*deal_with_single_quotes(char *input)
{
	int					i;
	t_next_arg_return	*res;

	i = 0;
	res = init_next_arg();
	while (input[i] && input[i] != '\'')
		i++;
	if (!input[i])
	{
		res->last_index = i;
		return (res);
	}
	res->arg = str_copy(input, i);
	res->last_index = i;
	return (res);
}

t_next_arg_return	*get_next_arg(char *input, t_settings *settings)
{
	int					start;
	t_next_arg_return	*res;

	res = malloc(sizeof(t_next_arg_return));
	res->arg = NULL;
	start = 0;
	while (input[start] && (input[start] == ' ' || input[start] == '\t'))
		start++;
	res->last_index = start;
	if (input[start] && !is_bash_special_char(input[start]))
	{
		res = handle_regular(input, start, settings, res);
		return (res);
	}
	if (start_with_spec(input, start))
		res = handle_spec_start(settings, res, input, start);
	return (res);
}

char	**split(char *input, t_settings *settings)
{
	char				**res;
	int					i;
	int					len;
	t_next_arg_return	*next_arg;

	res = malloc(sizeof(char *));
	res[0] = NULL;
	i = 0;
	len = 0;
	while (input[i] && (i <= 0 || input[i - 1]))
	{
		next_arg = get_next_arg(input + i, settings);
		i += next_arg->last_index;
		res = add_string_to_string_arr(next_arg->arg, res, len);
		res = add_wild_matches_if_needed(res, len);
		while (res && res[len])
			len++;
		free_next_arg_return(next_arg);
	}
	return (res);
}
