/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 16:41:30 by maissat           #+#    #+#             */
/*   Updated: 2025/02/26 18:08:57 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_without_quotes(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] != '"')
			count++;
		i++;
	}
	return (count);
}

void	destroy_node_quotes(t_data *data)
{
	//printf("in destroy node quotes\n");
	t_token	*prev;
	t_token	*current;
	// t_token	*tmp;
	int		i;

	current = data->list;
	prev = NULL;
	while (current)
	{
		i = 0;
		while (current->content[i] == '"')
			i++;
		if (current->content[i] == '\0')
		{
			// tmp = current;
			if (prev)
				prev->next = current->next;
			else
				data->list = current->next;
			current = current->next;
			//free(tmp->content);
			//free(tmp);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

char	**skip_quotes(t_data *data)
{	
	char 	**new_args;
	int		i;
	int		j;
	int		len;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	while (data->args[i])
		i++;
	new_args = ft_malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (data->args[i])
	{
		j = 0;
		k = 0;
		len = len_without_quotes(data->args[i]);
		new_args[i] = ft_malloc(sizeof(char) * (len + 1));
		while (data->args[i][j])
		{
			if(data->args[i][j] == '"' || data->args[i][j] == '\'')
				j++;
			else
			{
				new_args[i][k] = data->args[i][j];
				k++;
				j++;
			}
		}
		new_args[i][k] = '\0';
		i++;
	}
	new_args[i] = NULL;
	// free(data->args);
	return (new_args);
}