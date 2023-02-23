/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 03:30:06 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/23 03:31:56 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	disable_ctrlc(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	if (term.c_lflag & ECHOCTL)
		term.c_lflag ^= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}

void	change_ctrl_c(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag ^= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}

int	kill_children(t_settings *settings, int to_kill, int sig)
{
	static t_settings	*local_settings = NULL;
	int					i;

	if (!local_settings)
		local_settings = settings;
	if (to_kill && local_settings->pipex && local_settings->pipex->pid)
	{
		if (sig == SIGQUIT)
			printf("Quit: 3\n");
		i = 0;
		while (local_settings->pipex->pid[i])
		{
			kill(local_settings->pipex->pid[i], sig);
			i++;
		}
		return (1);
	}
	return (0);
}

void	interrupt_input(int sig)
{
	int	val;

	val = kill_children(NULL, 1, sig);
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("\0", 0);
		rl_redisplay();
		if (val)
			printf("\r");
	}
	if (sig == SIGQUIT)
	{
		rl_redisplay();
		printf("\b");
	}
}