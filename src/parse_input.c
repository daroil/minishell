/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 03:32:44 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/23 13:42:42 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_angulars(char **splitted_input)
{
	int	i;

	i = 0;
	while (splitted_input[i])
	{
		if (splitted_input[i][0] == '>' || splitted_input[i][0] == '<')
		{
			if ((splitted_input[i + 1] && splitted_input[i + 1][0] == '|')
				|| (splitted_input[i][0] == '|' && !splitted_input[i + 1]))
				return (printf("syntax error near unexpected token '|'\n")
					, 0);
			if (!splitted_input[i + 1])
				return (printf("syntax error near unexpected token 'newline'\n")
					, 0);
			if (splitted_input[i + 1][0] == '<')
				return (printf("syntax error near unexpected token '<'\n")
					, 0);
			if (splitted_input[i + 1][0] == '>')
				return (printf("syntax error near unexpected token '>'\n")
					, 0);
		}
		i++;
	}
	return (1);
}

int	check_basic_commands(char **splitted_input)
{
	if (strings_equal(splitted_input[0], "exit"))
		return (1);
	if (!splitted_input[0])
		return (1);
	if (strings_equal(splitted_input[0], "echo\0"))
		return (1);
	if (strings_equal(splitted_input[0], "pwd\0"))
		return (1);
	if (strings_equal(splitted_input[0], "cd\0"))
		return (1);
	if (strings_equal(splitted_input[0], "env\0"))
		return (1);
	if (strings_equal(splitted_input[0], "unset\0"))
		return (1);
	if (strings_equal(splitted_input[0], "export\0"))
		return (1);
	if (ft_strchr(splitted_input[0], '='))
		return (1);
	return (0);
}

int	basic_commands(char **splitted_input,
	char ***resplitted_input, t_settings *settings)
{
	if (strings_equal(splitted_input[0], "exit"))
		return (clear_splits(splitted_input, resplitted_input), 0);
	else if (!splitted_input[0])
		return (clear_splits(splitted_input, resplitted_input), 1);
	else if (strings_equal(splitted_input[0], "echo\0"))
		settings->last_exit_status = echo(splitted_input + 1);
	else if (strings_equal(splitted_input[0], "pwd\0"))
		settings->last_exit_status = pwd(splitted_input);
	else if (strings_equal(splitted_input[0], "cd\0"))
		settings->last_exit_status = cd(splitted_input, settings);
	else if (strings_equal(splitted_input[0], "env\0"))
		settings->last_exit_status = env(splitted_input, settings);
	else if (strings_equal(splitted_input[0], "unset\0"))
		settings->last_exit_status = unset(splitted_input, settings);
	else if (strings_equal(splitted_input[0], "export\0"))
		settings->last_exit_status = export(splitted_input, settings);
	else if (ft_strchr(splitted_input[0], '='))
		settings->last_exit_status = deal_with_equal_sign(splitted_input,
				settings);
	return (clear_splits(splitted_input, resplitted_input), 1);
}

int	parse_input(char *input, t_settings *settings)
{
	char	**splitted_input;
	char	***resplitted_input;

	if (!input)
		return (0);
	splitted_input = split(input, settings);
	if (!check_angulars(splitted_input) || !check_pipe(splitted_input))
	{
		ft_split_clear(splitted_input);
		return (1);
	}
	resplitted_input = resplit(splitted_input);
	if (count_resplitted(resplitted_input) == 1)
	{
		if ((check_basic_commands(splitted_input)))
			return (basic_commands(splitted_input, resplitted_input, settings));
		else
			return (pipex(splitted_input, resplitted_input, settings));
	}
	else
		return (pipex(splitted_input, resplitted_input, settings));
}

int	check_pipe(char **splitted_input)
{
	int	i;

	i = 0;
	while (splitted_input[i])
	{
		if (splitted_input[i][0] == '|' && !splitted_input[i + 1])
			return (printf("syntax error near unexpected token '|'\n")
				, 0);
		if (splitted_input[i][0] == '|' && splitted_input[i + 1][0] == '|')
			return (printf("syntax error near unexpected token '||'\n")
				, 0);
		i++;
	}
	return (1);
}
