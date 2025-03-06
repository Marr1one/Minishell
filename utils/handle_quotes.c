/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:15:10 by maissat           #+#    #+#             */
/*   Updated: 2025/03/06 17:11:19 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_global_quotes(char *str)
{
	int		i;
	int		count;
	
	i = 0;
	count = 0;
	
	while (str[i])
	{
		
		if (str[i] == '"' || str[i] == '\'')
			count++;
		i++;
	}
	return (count);
}

char	*rm_qts_str(char *str)
{
	int	i;
	int	j;
	int	nb_quotes = count_global_quotes(str);
	char	*new_str;

	i = 0;
	while (str[i])
		i++;
	new_str = ft_malloc (sizeof(char) * (i - nb_quotes + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '"' && str[i] != '\'')
		{
			new_str[j] = str[i];
			i++;
			j++;
		}
		else
			i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

void	rm_qts_nodes(t_data *data)
{
	t_token	*current;

	current = data->list;
	while (current)
	{
		current->content = rm_qts_str(current->content);
		current = current->next;
	}		
}