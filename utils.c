/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbritani <sbritani@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 18:43:10 by sbritani          #+#    #+#             */
/*   Updated: 2023/02/16 14:50:33 by sbritani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_str_join_free_first(char *str1, char *str2)
{
	char	*res;
	int		len1;
	int		len2;
	int		i;

	len1 = ft_strlen(str1);
	len2 = ft_strlen(str2);
	if (!str1 && !str2)
		return NULL;
	res = malloc(sizeof(char) * (len1 + len2 + 1));
	i = -1;
	if (str1)
	{
		while (str1[++i])
			res[i] = str1[i];
	}
	i = -1;
	if (str2)
	{
		while (str2[++i])
			res[i + len1] = str2[i];
	}
	res[len1 + len2] = '\0';
	free(str1);
	return (res);
}

char	*ft_str_join_free_both(char *str1, char *str2)
{
	char	*res;
	int		len1;
	int		len2;
	int		i;

	len1 = ft_strlen(str1);
	len2 = ft_strlen(str2);
	if (!str1 && !str2)
		return NULL;
	res = malloc(sizeof(char) * (len1 + len2 + 1));
	i = -1;
	if (str1)
	{
		while (str1[++i])
			res[i] = str1[i];
	}
	i = -1;
	if (str2)
	{
		while (str2[++i])
			res[i + len1] = str2[i];
	}
	res[len1 + len2] = '\0';
	free(str1);
	free(str2);
	return (res);
}

int	strings_equal(char *str1, char *str2)
{
	int	i;
	
	if (!str1 && !str2)
		return (1);
	if (!str1 || !str2)
		return (0);
	i = 0;
	while (str1[i] && str2[i] && str1[i] == str2[i])
		i++;
	if (str1[i] != str2[i])
		return (0);
	return (1);
}

char *str_copy(char *str, int n)
{
	char	*res;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	if (n < 0)
	{
		while (str[i++])
			;
		res = malloc(sizeof(char) * i);
		i = -1;
		while (str[++i])
			res[i] = str[i];
		res[i] = '\0';
	}
	else
	{
		res = malloc(sizeof(char) * (n + 1));
		i = -1;
		while (str[++i] && i < n)
			res[i] = str[i];
		res[i] = '\0';
	}
	return (res);
}

char	**add_string_to_string_arr(char *str, char **arr, int n)
{
	char **res;
	int	i;

	i = 0;
	res = malloc(sizeof(char *) * (n + 2));
	while (i < n)
	{
		res[i] = arr[i];
		i++;
	}
	res[i] = str_copy(str, -1);
	res[i + 1] = NULL;
	free(arr);
	return (res);
}

char	**remove_string_from_str_arr(char **arr, int n)
{
	char	**res;
	int	i;

	i = 0;
	while (arr[i++])
		;
	res = malloc (sizeof(char *) * (i - 1));
	i = -1;
	while (arr[++i])
	{
		if (i < n)
			res[i] = arr[i];
		else if (i > n)
			res[i - 1] = arr[i];
	}
	res[i - 1] = NULL;
	free(arr[n]);
	free(arr);
	return (res);
}