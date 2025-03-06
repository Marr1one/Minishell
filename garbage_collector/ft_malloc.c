/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:18:27 by maissat           #+#    #+#             */
/*   Updated: 2025/03/06 17:14:42 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_malloc_node	*findlast_malloc(t_malloc_node	*list)
{
	if (!list)
		return (NULL);
	while (list->next != NULL)
		list = list->next;	
	return (list);
}

void	free_all(t_malloc *gc)
{
	t_malloc_node	*current;
	t_malloc_node	*tmp;
	int				i;

	i = 0;
	if (!gc)
	{
		printf("gc est null\n");
		return ;
	}
	current = gc->first;
	while (current)
	{
		tmp = current->next;
		free(current->addr);
		free(current);
		current = tmp;
		i++;
	}
	printf("nombre d'adresse libere : %d\n", i);
	gc->first = NULL;
	free(gc);
}

t_malloc	**get_gc(void)
{
	static t_malloc	*gc = NULL;
	if (!gc)
	{
		gc = malloc(sizeof(t_malloc));
		if (!gc)
			return (NULL);
		if (gc)
			gc->first = NULL;
	}
	return (&gc);
}

void	*ft_malloc(size_t size)
{
	//printf("in ft_malloc\n");
	t_malloc	**gc;
	t_malloc_node	*new_node;
	void			*ptr;
	
	gc = get_gc();
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
	if ((*gc)->first == NULL)
	{
		(*gc)->first = new_node;
	}
	else
		findlast_malloc((*gc)->first)->next = new_node;
	return (ptr);
}