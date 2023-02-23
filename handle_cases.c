/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cases.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 03:02:07 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/23 03:05:39 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	start_with_more(char *input, int start, t_next_arg_return *res)
{
	if (input[start + 1] && input[start + 1] == '>')
	{
		res->last_index = start + 2;
		res->arg = str_copy(">>\0", -1);
	}
	else if (input[start + 1] && input[start + 1] == '|')
	{
		res->last_index = start + 2;
		res->arg = str_copy(">\0", -1);
	}
	else
	{
		res->last_index = start + 1;
		res->arg = str_copy(">\0", -1);
	}
}

void	start_with_less(char *input, int start, t_next_arg_return *res)
{
	if (input[start + 1] && input[start + 1] == '<')
	{
		res->last_index = start + 2;
		res->arg = str_copy("<<\0", -1);
	}
	else
	{
		res->last_index = start + 1;
		res->arg = str_copy("<\0", -1);
	}
}

void	start_with_pipe(char *input, int start, t_next_arg_return *res)
{
	res->last_index = start + 1;
	res->arg = str_copy("|\0", -1);
}

t_next_arg_return	*handle_regular(char *input, int start,
		t_settings *settings, t_next_arg_return *res)
{
	int	i;

	i = start;
	res->last_index = i;
	while (input[i] && input[i] != ' ' && input[i] != '\t')
	{
		while (input[i] && (!is_bash_special_char(input[i])) || input[i] == '=')
			i++;
		res->arg = ft_str_join_free_both(res->arg,
				str_copy(input + res->last_index, i - res->last_index));
		res->last_index = i;
		if (dollar_or_quote(input, i))
			i += for_spec_char_return_index(input, res, settings, i);
		else if (input[i] == '|' || input[i] == '>' || input[i] == '<')
			return (res);
	}
	res -> last_index = i;
	return (res);
}

t_next_arg_return	*handle_spec_start(t_settings *settings,
		t_next_arg_return *res, char *input, int start)
{
	if (input[start] && input[start] == '$')
	{
		free_next_arg_return(res);
		res = deal_with_dollar(input + start + 1, settings);
		res->last_index += start + 1;
	}
	if (input[start] && input[start] == '"')
	{
		free_next_arg_return(res);
		res = deal_with_double_quotes(input + start + 1, settings);
		res->last_index += start + 2;
	}
	if (input[start] && input[start] == '\'')
	{
		free_next_arg_return(res);
		res = deal_with_single_quotes(input + start + 1, settings);
		res->last_index += start + 2;
	}
	if (input[start] && input[start] == '|')
		start_with_pipe(input, start, res);
	if (input[start] && input[start] == '>')
		start_with_more(input, start, res);
	if (input[start] && input[start] == '<')
		start_with_less(input, start, res);
	return (res);
}
