/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:10:49 by maissat           #+#    #+#             */
/*   Updated: 2025/03/04 16:58:56 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**list_to_args(t_data *data)
{
	t_token *current;
	char	**new_args;
	int		i;
	
	i = 0;
	new_args = ft_malloc(sizeof(char *) * (list_len(data->list) + 1));
	if (!new_args)
		return (NULL);
	current = data->list;
	while (current)
	{
		new_args[i] = ft_strdup(current->content);
		i++;
		current = current->next;
	}
	new_args[i] = NULL;
	return (new_args);
}

int	list_len(t_token *list)
{
	int	i;
	t_token *current;
	
	i = 0;
	current = list;
	while (current)
	{
		current = current->next;
		i++;
	}
	return (i);
}


int	get_nbr_node(t_token *list)
{
	int	i;
	t_token *current;
	
	i = 0;
	current = list;
	while (current != NULL)
	{
		i++;
		current = current->next;
	}
	return (i);
}