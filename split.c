/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 19:18:48 by sbritani          #+#    #+#             */
/*   Updated: 2023/02/03 19:46:08 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_bash_special_char(char c)
{
	int			i;
	static char	special_chars[] = "'\"$&= \0";

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


t_next_arg_return	*deal_with_dollar(char *input, t_settings *settings)
{
	t_next_arg_return *res;
	char *mid_line;
	int	i;

	res = init_next_arg();
	i = 0;
	if (*input == '?')
	{
		res->arg = ft_itoa(settings->last_exit_status);
		res->last_index++;
		return (res);
	}
	while (input[i] && !is_bash_special_char(input[i]))
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
	t_next_arg_return	*mid_dollar_res;
	i = 0;
	res = init_next_arg();
	res->last_index = i;
	while(input[i] && input[i] != '"')
	{
		while (input[i] && input[i] != '"' && input[i] != '$')
			i++;
		// printf("res->last_index = %d i is %d\n", res->last_index, i);
		res->arg = ft_str_join_free_both(res->arg, str_copy(input + res->last_index, i - res->last_index));
		res->last_index = i;
		// printf("before dealing with dollar is %s  %d\n", res->arg, res->last_index);
		if (input[i] == '$')
		{
			mid_dollar_res = deal_with_dollar(input + i + 1, settings);
			res->arg = ft_str_join_free_first(res->arg, mid_dollar_res->arg);
			i += mid_dollar_res->last_index + 1;
			res->last_index += mid_dollar_res->last_index + 1;
			free_next_arg_return(mid_dollar_res);
		}
		// printf("after dealing with dollar is %s  %d\n", res->arg, res->last_index);
		// printf("last index = %d\n", res->last_index);
		if (input[i] == '"')
			return (res);
	}
	if (!input[i])
	free_next_arg_return(res);
	res = init_next_arg();
	res->last_index = i;
	return (res);
}

t_next_arg_return	*deal_with_single_quotes(char *input, t_settings *settings)
{
	int	i;
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

t_next_arg_return *get_next_arg(char *input, t_settings *settings)
{
	int	i;
	int	start;
	t_next_arg_return *mid_dollar_res;
	t_next_arg_return *res;

	res = malloc(sizeof(t_next_arg_return));
	res->arg = NULL;
	start = 0;
	while (input[start] && (input[start] == ' ' || input[start] == '\t'))
		start++;
	res->last_index = start;
	if (input[start] && !is_bash_special_char(input[start]))
	{
		i = start;
		res->last_index = i;
		while(input[i] && input[i] != ' ')
		{
			while (input[i] && (!is_bash_special_char(input[i])) || input[i] == '=')
				i++;
			res->arg = ft_str_join_free_both(res->arg, str_copy(input + res->last_index, i - res->last_index));
			res->last_index = i;
			if (input[i] == '$')
			{
				mid_dollar_res = deal_with_dollar(input + i + 1, settings);
				res->arg = ft_str_join_free_first(res->arg, mid_dollar_res->arg);
				i += mid_dollar_res->last_index + 1;
				res->last_index += mid_dollar_res->last_index + 1;
				free_next_arg_return(mid_dollar_res);
			}
			else if (input[i] == '"')
			{
				mid_dollar_res = deal_with_double_quotes(input + i + 1, settings);
				res->arg = ft_str_join_free_first(res->arg, mid_dollar_res->arg);
				i += mid_dollar_res->last_index + 2;
				printf("%d\n", i);
				res->last_index += mid_dollar_res->last_index + 2;
				free_next_arg_return(mid_dollar_res);
			}
			else if (input[i] == '\'')
			{
				mid_dollar_res = deal_with_single_quotes(input + i + 1, settings);
				res->arg = ft_str_join_free_first(res->arg, mid_dollar_res->arg);
				i += mid_dollar_res->last_index + 2;
				printf("%d\n", i);
				res->last_index = mid_dollar_res->last_index + 1;
				free_next_arg_return(mid_dollar_res);
			}
		}
		res -> last_index = i;
		return (res);
	}
	if (input[start] && input[start] == '$')
	{
		free_next_arg_return(res);
		res = deal_with_dollar(input + start + 1, settings);
		res->last_index += start + 1;
	}
	if(input[start] && input[start] == '"')
	{
		free_next_arg_return(res);
		res = deal_with_double_quotes(input + start + 1, settings);
		res->last_index += start + 2;
	}
	if(input[start] && input[start] == '\'')
	{
		free_next_arg_return(res);
		res = deal_with_single_quotes(input + start + 1, settings);
		res->last_index += start + 2;
		printf("%s\n", res->arg);
	}
	return (res);
	
}

char **split(char *input, t_settings *settings)
{
	char **res;
	int	i;
	int	len;
	t_next_arg_return *next_arg;
	
	res = malloc(sizeof(char *));
	res[0] = NULL;
	i = 0;
	len=0;
	while (input[i])
	{
		next_arg = get_next_arg(input + i, settings);
		i += next_arg->last_index;
		res = add_string_to_string_arr(next_arg->arg, res, len);
		free_next_arg_return(next_arg);
		len++;
	}
	//for i in res:
	//		if i == "|"
	//			join
	return (res);
}