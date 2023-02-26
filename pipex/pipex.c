/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 03:26:05 by dhendzel          #+#    #+#             */
/*   Updated: 2023/02/26 16:54:32 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**unite_env(t_dict *dict)
{
	int		i;
	char	**res;
	char	*string;

	res = NULL;
	i = 0;
	while (i < dict->len)
	{
		string = ft_str_join_free_both(ft_str_join_free_first(
					str_copy(dict->keys[i], -1), "=\0"),
				str_copy(dict->values[i], -1));
		res = add_string_to_string_arr(string, res, i);
		free(string);
		i++;
	}
	return (res);
}

void	pipex_init(t_settings *settings, char ***resplitted_input)
{
	int	pipe_num;

	change_ctrl_c();
	settings->pipex->number_of_pipes = count_resplitted(resplitted_input) - 1;
	pipe_num = settings->pipex->number_of_pipes;
	settings->pipex->pid = malloc(sizeof(pid_t) * (pipe_num + 1));
	settings->pipex->truby = malloc(sizeof(int *) * (pipe_num + 1));
	settings->pipex->i = 0;
	settings->pipex->redirect_input = 0;
	settings->pipex->redirect_output = 0;
	while (settings->pipex->i < settings->pipex->number_of_pipes)
	{
		settings->pipex->truby[settings->pipex->i] = malloc(sizeof(int) * 2);
		pipe(settings->pipex->truby[settings->pipex->i]);
		settings->pipex->i++;
	}
	settings->pipex->truby[settings->pipex->i] = NULL;
}

void	clean_and_wait_pipex(t_settings *settings)
{
	int	exit_code;

	settings->pipex->i = 0;
	while (settings->pipex->truby[settings->pipex->i])
	{
		close(settings->pipex->truby[settings->pipex->i][0]);
		close(settings->pipex->truby[settings->pipex->i][1]);
		free(settings->pipex->truby[settings->pipex->i]);
		settings->pipex->i++;
	}
	free(settings->pipex->truby);
	settings->pipex->i = 0;
	while (settings->pipex->i <= settings->pipex->number_of_pipes)
	{
		waitpid(settings->pipex->pid[settings->pipex->i],
			&exit_code, 0);
		settings->last_exit_status = WEXITSTATUS(exit_code);
		if (WIFSIGNALED(exit_code) && WTERMSIG(exit_code) == SIGSEGV)
		{
			printf("Segmentation is your Fault: 11\n");
			settings->last_exit_status = 139;
		}
		settings->pipex->i++;
	}
	free(settings->pipex->pid);
}

int	pipex(char **splitted_input, char ***resplitted_input, t_settings *settings)
{
	t_pipex	pipex;
	char	**something;

	something = unite_env(settings->exported_env);
	settings->pipex = &pipex;
	pipex_init(settings, resplitted_input);
	pipex.i = 0;
	while (pipex.i <= pipex.number_of_pipes)
	{
		single_pipe(resplitted_input[pipex.i], pipex, something, settings);
		if (string_in_array_of_strings("<<", resplitted_input[pipex.i]))
			waitpid(pipex.pid[pipex.i], NULL, 0);
		pipex.i++;
	}
	clean_and_wait_pipex(settings);
	disable_ctrlc();
	ft_split_clear(splitted_input);
	ft_split_clear(something);
	free_resplitted(resplitted_input);
	settings->pipex = NULL;
	return (1);
}
