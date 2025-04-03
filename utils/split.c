/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:35:34 by maissat           #+#    #+#             */
/*   Updated: 2025/04/04 00:00:24 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	init_split(char *str, char c, char ***res)
{
	int	word_count;

	if (!str)
		return (0);
	word_count = countword(str, c);
	*res = ft_malloc((word_count + 1) * sizeof(char *));
	if (!(*res))
		return (0);
	return (1);
}

static int	fill_words(char **res, char *str, char c)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	start = -1;
	while (str[i])
	{
		if (str[i] != c && start < 0)
			start = i;
		if (str[i + 1] == '\0' && start >= 0)
		{
			res[j++] = custom_worddup(str, start, i);
			break ;
		}
		else if (str[i] == c && start >= 0)
		{
			res[j++] = worddup(str, start, i);
			start = -1;
		}
		i++;
	}
	res[j] = NULL;
	return (1);
}

char	**ft_split(char *str, char c)
{
	char	**res;

	if (!init_split(str, c, &res))
		return (NULL);
	fill_words(res, str, c);
	return (res);
}
