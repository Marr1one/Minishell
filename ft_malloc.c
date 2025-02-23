/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:18:27 by maissat           #+#    #+#             */
/*   Updated: 2025/02/23 18:54:07 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_malloc_node	*findlast_malloc(t_malloc_node	*list)
{
	if (!list)
		return (NULL);
	while (list->next != NULL)
		list = list->next;	
	return (list);
}

void	*ft_malloc(size_t size)
{
	static t_malloc	*gc = NULL;
	t_malloc_node	*new_node;
	void			*ptr;
	
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	new_node = malloc(sizeof(t_malloc_node));
	if (!new_node)
	{
		free(ptr);
		return (NULL);
	}
	new_node->addr = ptr;
	new_node->next = NULL;
	if (!gc)
	{
		gc = malloc(sizeof(t_malloc));
		if (!gc)
			return (NULL);
		gc->first = new_node;
	}
	else
		findlast_malloc(gc->first)->next = new_node;
	return (ptr);
}