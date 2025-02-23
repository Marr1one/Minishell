/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:35:34 by maissat           #+#    #+#             */
/*   Updated: 2025/02/23 18:44:48 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*worddup(char *str, int start, int end)
{
	int		i;
	char	*dup;

	i = 0;
	dup = ft_malloc((end - start + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	while (start < end)
		dup[i++] = str[start++];
	dup[i] = '\0';
	return (dup);
}

int	countword(char *str, char c)
{
	int	counter;
	int	check;

	counter = 0;
	check = 0;
	if (!str)
		return (0);
	while (*str)
	{
		if (*str != c && check == 0)
		{
			counter++;
			check = 1;
		}
		if (*str == c)
			check = 0;
		str++;
	}
	return (counter);
}

char	**ft_split(char *str, char c)
{
	char	**res;
	int		idx;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	idx = -1;
	i = -1;
	j = 0;
	res = ft_malloc((countword(str, c) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	while (++i < ft_strlen(str) + 1)
	{
		if (str[i] != c && idx < 0)
			idx = i;
		if ((str[i] == c || i == ft_strlen(str)) && idx >= 0)
		{
			res [j++] = worddup(str, idx, i);
			idx = -1;
		}
	}
	res[j] = 0;
	return (res);
}
