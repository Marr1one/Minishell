/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chaining.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:45:04 by maissat           #+#    #+#             */
/*   Updated: 2025/02/23 18:40:58 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//int	count_args(char **tab)
//{
//	int	i;

//	i = 0;
//	while (tab[i])
//		i++;
//	return (i);
//}

//void	show_content(t_token *token)
//{
//	t_token *current;

//	current = token;
//	while (current != NULL)
//	{
//		printf("token %d content : %s, type ;%d\n", current->index,current->content, current->type);
//		current = current->next;
//	}
//}

t_token	*findlast_token(t_token	*list)
{
	if (!list)
		return (NULL);
	while (list->next != NULL)
		list = list->next;	
	return (list);
}


t_token	*add_chained_list(t_data *data)
{
	int	i;
	t_token	*list;
	t_token *new_node;
	
	list = NULL;
	i = 0;
	while(data->args[i])
	{
		new_node = ft_malloc(sizeof(t_token));
		if (!new_node)
			return (NULL);
		new_node->content = ft_strdup(data->args[i]);
		new_node->index = i;
		if (ft_strlcmp(new_node->content, ">") == 0)
			new_node->type = REDIRECTION;
		else
			new_node->type = WORD;
		new_node->next = NULL;
		if (!list)
			list = new_node;
		else	
			findlast_token(list)->next = new_node;
		i++;
	}
	//show_content(list);
	return (list);
}
