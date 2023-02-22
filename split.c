/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 19:18:48 by sbritani          #+#    #+#             */
/*   Updated: 2023/02/22 13:18:13 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	t_next_arg_return	*mid_dollar_res;

	i = 0;
	res = init_next_arg();
	res->last_index = i;
	while(input[i] && input[i] != '"')
	{
		while (input[i] && input[i] != '"' && input[i] != '$')
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
		if (input[i] == '"')
			return (res);
	}
	if (!input[i])
	{
		free_next_arg_return(res);
		res = init_next_arg();
		res->last_index = i;
		return (res);
	}
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

int	for_spec_char_return_index(char *input, t_next_arg_return *res, t_settings *settings, int i)
{
	t_next_arg_return *mid_dollar_res;

	if (input[i] == '$')
	{
		mid_dollar_res = deal_with_dollar(input + i + 1, settings);
		res->arg = ft_str_join_free_first(res->arg, mid_dollar_res->arg);
		i += mid_dollar_res->last_index + 1;
		res->last_index += mid_dollar_res->last_index + 1;
	}
	else if (input[i] == '"')
	{
		mid_dollar_res = deal_with_double_quotes(input + i + 1, settings);
		res->arg = ft_str_join_free_first(res->arg, mid_dollar_res->arg);
		i += mid_dollar_res->last_index + 2;
		res->last_index += mid_dollar_res->last_index + 2;
	}
	else if (input[i] == '\'')
	{
		mid_dollar_res = deal_with_single_quotes(input + i + 1, settings);
		res->arg = ft_str_join_free_first(res->arg, mid_dollar_res->arg);
		i += mid_dollar_res->last_index + 2;
		res->last_index += mid_dollar_res->last_index + 1;
	}
	free_next_arg_return(mid_dollar_res);
	return (i);
}

t_next_arg_return *get_next_arg(char *input, t_settings *settings)
{
	int	i;
	int	start;
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
		while(input[i] && input[i] != ' ' && input[i] != '\t')
		{
			while (input[i] && (!is_bash_special_char(input[i])) || input[i] == '=')
				i++;
			res->arg = ft_str_join_free_both(res->arg, str_copy(input + res->last_index, i - res->last_index));
			res->last_index = i;
			if (dollar_or_quote(input, i))
				i += for_spec_char_return_index(input, res, settings, i);
			else if (input[i] == '|' || input[i] == '>' || input[i] == '<')
				return (res);
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
	}
	if (input[start] && input[start] == '|')
	{
		res->last_index = start + 1;
		res->arg = str_copy("|\0", -1);
	}
	if (input[start] && input[start] == '>')
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
	if (input[start] && input[start] == '<')
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
	//for i in res:
	//		if i == "|"
	//			join
	return (res);
}

char **copy_str_array(char **array, int n)
{
	char **res;
	int i;
	
	if (!array)
		return (NULL);
	i = 0;
	while (array[i] && (i < n || n < 0))
		i++;
	res = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (array[i] && (i < n || n < 0))
	{
		res[i] = str_copy(array[i], -1);
		i++;
	}
	res[i] = NULL;
	return (res);
}

char	***add_string_array_to_array_of_string_arrays(char ***old, char **to_add)
{
	char ***res;
	int	i;
	if (!old)
	{
		res = malloc(sizeof(char **) * 2);
		res[0] = to_add;
		res[1] = NULL;
		return (res);
	}
	i = 0;
	while (old[i])
		i++;
	res = malloc(sizeof(char **) * (i + 2));
	i = 0;
	while(old[i])
	{
		res[i] = old[i];
		i++;
	}
	res[i] = to_add;
	res[i + 1] = NULL;
	free(old);
	return (res);
}

char ***resplit(char **splitted)
{
	char	***res;
	int		prev_start;
	int		i;

	prev_start = 0;
	res = NULL;
	i = prev_start;
	while (splitted[i])
	{
		if (!strings_equal(splitted[i], "|\0"))
			i++;
		else
		{
			res = add_string_array_to_array_of_string_arrays(res, copy_str_array(splitted + prev_start, i - prev_start));
			prev_start = i + 1;
			i++;
		}
	}
	res = add_string_array_to_array_of_string_arrays(res, copy_str_array(splitted + prev_start, i - prev_start));
	i++;
	res = add_string_array_to_array_of_string_arrays(res, NULL);
	return (res);
}

int		count_resplitted(char ***resplitted)
{
	int i;
	i = 0;
	if (!resplitted)
		return (0);
	while (resplitted[i])
		i++;
	return (i);
}

void	free_resplitted(char ***resplitted)
{
	int i;
	// printf("freeing resplitted\n");
	if (!resplitted)
		return ;
	i = 0;
	while (resplitted [i])
	{
		ft_split_clear(resplitted[i]);
		i++;
	}
	
	// printf("freed resplitted\n");
	free(resplitted);
}