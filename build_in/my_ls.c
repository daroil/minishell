/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhendzel <dhendzel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 16:10:29 by sbritani          #+#    #+#             */
/*   Updated: 2023/02/23 04:02:37 by dhendzel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**my_ls(void)
{
	DIR				*folder;
	struct dirent	*entry;
	int				files;
	char			**res;

	files = 0;
	res = NULL;
	folder = opendir(".");
	if (folder == NULL)
	{
		return (NULL);
	}
	while (1)
	{
		files ++;
		entry = readdir(folder);
		if (!entry)
		{
			closedir(folder);
			return (res);
		}
		res = add_string_to_string_arr(entry->d_name, res, files - 1);
	}
}

int	matches_wild(char *str, char *wild)
{
	int	i;

	i = 0;
	while (str[i] && wild[i] && str[i] == wild[i])
		i++;
	if (!str[i] && !wild[i])
		return (1);
	if (wild[i] == '*')
	{
		return ((str[i] && matches_wild(str + i + 1, wild + i))
			|| (wild[i] && matches_wild(str + i, wild + i + 1))
			|| (str[i] && wild[i]
				&& matches_wild(str + i + 1, wild + i + 1)));
	}
	return (0);
}

char	**get_all_matching_files(char *wild)
{
	char	**res;
	char	**mid;
	int		i;
	int		counter;

	i = 0;
	counter = 0;
	res = NULL;
	mid = my_ls();
	while (mid[i])
	{
		if (matches_wild(mid[i], wild))
		{
			counter++;
			res = add_string_to_string_arr(mid[i], res, counter - 1);
		}
		i++;
	}
	if (mid && mid[0])
		ft_split_clear(mid);
	else if (mid)
		free(mid);
	return (res);
}

char	**add_wild_matches_if_needed(char **splitted_input, int len_splitted)
{
	int		i;
	char	**mid;

	if (splitted_input[len_splitted]
		&& ft_str_chr(splitted_input[len_splitted], '*'))
	{
		mid = get_all_matching_files(splitted_input[len_splitted]);
		free(splitted_input[len_splitted]);
		splitted_input[len_splitted] = NULL;
		i = 0;
		while (mid && mid[i])
		{
			splitted_input = add_string_to_string_arr(mid[i],
					splitted_input, len_splitted + i);
			i++;
		}
		if (mid)
			ft_split_clear(mid);
	}
	return (splitted_input);
}
