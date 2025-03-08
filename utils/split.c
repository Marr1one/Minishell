/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:35:34 by maissat           #+#    #+#             */
/*   Updated: 2025/03/08 17:52:33 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


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



char	*custom_worddup(char *str, int start, int end)
{
	int		i;
	char	*dup;

	i = 0;
	dup = ft_malloc((end - start + 2) * sizeof(char));
	if (!dup)
		return (NULL);
	while (start <= end)
		dup[i++] = str[start++];
	dup[i] = '\0';
	return (dup);
}

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

char	**ft_split(char *str, char c)
{
	char	**res;
	int		idx;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	idx = -1;
	i = 0;
	j = 0;
	printf("countword = %d\n", countword(str, c));
	res = ft_malloc((countword(str, c) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	while (str[i])
	{
		
		if (str[i] != c && idx < 0 )
		{
			printf("on entre dans un mot, on voit le caractere : {%c}\n", str[i]);
			idx = i;
		}
		if (str[i + 1] == '\0')
		{
			res[j++] = custom_worddup(str, idx, i);
			break;
		}
		else if ((str[i] == c  && idx >= 0))
		{
			printf("on est a la fin du mot :{ %c}\n", str[i]);
			res [j++] = worddup(str, idx, i);
			idx = -1;
		}
		i++;
	}
	res[j] = NULL;
	return (res);
}


char	**custom_split(char *str, char c)
{
	char	**res;
	int		idx;
	int		i;
	int		j;
	int		in_word;
	int		quote;

	if (!str)
		return (NULL);
	idx = -1;
	i = 0;
	j = 0;
	in_word = 0;
	quote = 0;
	res = ft_malloc((countword(str, c) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '"')
		{
			if (quote == 0 && in_word == 0)
			{
				printf(" guillemet ouvrante\n");
				idx = i;
				quote = 1;
			}
			else if (quote == 1)
			{
				printf("guillemet fermante\n");
				res[j++] = custom_worddup(str, idx, i);
				idx = -1;
				quote = 0;
			}
		}
		else 
		{	
			if (str[i] != c && idx < 0 && quote != 1)
			{
				in_word = 1;
				idx = i;
			}
			else if ((str[i] == c  && idx >= 0  && quote != 1))
			{
				res [j++] = worddup(str, idx, i);
				idx = -1;
				in_word = 0;
			}
		}
		i++;
	}
	if (idx >= 0)
        res[j++] = worddup(str, idx, i);
	res[j] = NULL;
	return (res);
}
