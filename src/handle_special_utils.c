/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_special_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 03:11:33 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/23 13:52:35 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	dollar_or_quote(char *input, int i)
{
	if (input[i] == '$')
		return (1);
	else if (input[i] == '"')
		return (1);
	else if (input[i] == '\'')
		return (1);
	else
		return (0);
}

int	for_spec_char_return_index(char *input,
		t_next_arg_return *res, t_settings *settings, int i)
{
	t_next_arg_return	*mid_dollar_res;

	if (input[i] == '$')
	{
		mid_dollar_res = deal_with_dollar(input + i + 1, settings);
		res->arg = ft_str_join_free_first(res->arg, mid_dollar_res->arg);
		i = mid_dollar_res->last_index + 1;
		res->last_index += mid_dollar_res->last_index + 1;
	}
	else if (input[i] == '"')
	{
		mid_dollar_res = deal_with_double_quotes(input + i + 1, settings);
		res->arg = ft_str_join_free_first(res->arg, mid_dollar_res->arg);
		i = mid_dollar_res->last_index + 2;
		res->last_index += mid_dollar_res->last_index + 2;
	}
	else
	{
		mid_dollar_res = deal_with_single_quotes(input + i + 1);
		res->arg = ft_str_join_free_first(res->arg, mid_dollar_res->arg);
		i = mid_dollar_res->last_index + 2;
		res->last_index += mid_dollar_res->last_index + 1;
	}
	free_next_arg_return(mid_dollar_res);
	return (i);
}

int	start_with_spec(char *input, int start)
{
	if (input[start] && input[start] == '$')
		return (1);
	if (input[start] && input[start] == '"')
		return (1);
	if (input[start] && input[start] == '\'')
		return (1);
	if (input[start] && input[start] == '|')
		return (1);
	if (input[start] && input[start] == '>')
		return (1);
	if (input[start] && input[start] == '<')
		return (1);
	return (0);
}

t_next_arg_return	*no_input(t_next_arg_return	*res, int i)
{
	free_next_arg_return(res);
	res = init_next_arg();
	res->last_index = i;
	return (res);
}

int	is_bash_special_char(char c)
{
	int			i;
	static char	special_chars[] = "'\"$=|<> \0";

	i = 0;
	while (special_chars[i])
	{
		if (c == special_chars[i])
		{
			return (1);
		}
		i++;
	}
	return (0);
}
