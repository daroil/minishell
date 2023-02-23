/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 17:53:20 by sbritani          #+#    #+#             */
/*   Updated: 2023/02/23 13:43:40 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	finish(t_settings *settings, char *input)
{
	clear_settings(settings);
	free(input);
}

void	my_readline(t_settings *settings)
{
	char				*prompt;
	static t_settings	*local_settings = NULL;

	if (!local_settings)
		local_settings = settings;
	prompt = ft_str_join_free_both(str_copy("\0", -1),
			ft_str_join_free_first(cur_dir(), "> \0"));
	local_settings->input = readline(prompt);
	free(prompt);
}

void	shell_init(t_settings *settings)
{
	kill_children(settings, 0, 0);
	disable_ctrlc();
	settings->last_working_directory = cur_dir();
	signal(SIGINT, interrupt_input);
	signal(SIGQUIT, interrupt_input);
	my_readline(settings);
}

void	shell(char *envp[])
{
	t_settings	*settings;

	settings = create_setttings(envp);
	shell_init(settings);
	if (!parse_input(settings->input, settings))
		return (finish(settings, settings->input));
	while (settings->input)
	{
		if (!strings_equal(settings->last_cmd, settings->input))
			add_history(settings->input);
		if (settings->input)
		{
			if (settings->last_cmd)
				free_last_cmd(settings);
			settings->last_cmd = str_copy(settings->input, -1);
		}
		free(settings->input);
		my_readline(NULL);
		if (!parse_input(settings->input, settings))
			return (finish(settings, settings->input));
	}
	finish(settings, settings->input);
}

int	main(int argc, char **argv, char **envp)
{
	if (argc && argv)
		shell(envp);
	system("leaks shell");
	return (0);
}
