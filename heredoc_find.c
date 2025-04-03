/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_find.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:04:46 by maissat           #+#    #+#             */
/*   Updated: 2025/04/03 21:09:17 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Renvoie le dernier heredoc trouvé dans la liste des fichiers.
t_file	*find_last_heredoc(t_file *files, int *last_index)
{
	t_file	*current;
	t_file	*last;
	int		current_index;

	current = files;
	last = NULL;
	current_index = 0;
	*last_index = -1;
	while (current)
	{
		if (current->mode == HEREDOC)
		{
			last = current;
			*last_index = current_index;
		}
		current = current->next;
		current_index++;
	}
	return (last);
}

// Cherche récursivement un heredoc existant dans la liste.
t_file	*find_existing_heredoc(t_file *node)
{
	if (node == NULL)
		return (NULL);
	if (node->mode == HEREDOC)
		return (node);
	return (find_existing_heredoc(node->next));
}

// Renvoie récursivement le dernier noeud d'une liste de fichiers.
t_file	*find_last_node(t_file *node)
{
	if (node == NULL)
		return (NULL);
	if (node->next == NULL)
		return (node);
	return (find_last_node(node->next));
}
