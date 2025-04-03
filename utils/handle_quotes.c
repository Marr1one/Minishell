/* ************************************************************************** */
/*                                                                          */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:15:10 by maissat           #+#    #+#             */
/*   Updated: 2025/04/03 18:39:09 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_quotes(char c, t_data *data)
{
	if (c == '"' && data->in_quote != 2)
	{
		if (data->in_quote == 1)
			data->in_quote = 0;
		else
			data->in_quote = 1;
		return (1);
	}
	else if (c == '\'' && data->in_quote != 1)
	{
		if (data->in_quote == 2)
			data->in_quote = 0;
		else
			data->in_quote = 2;
		return (1);
	}
	return (0);
}

int	ft_atoi(char *str)
{
	int	n;
	int	i;

	n = 0;
	i = 0;
	while (str[i])
	{
		n = n * 10 + (str[i] - '0');
		i++;
	}
	return (n);
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
