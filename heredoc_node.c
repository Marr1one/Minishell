/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:03:10 by maissat           #+#    #+#             */
/*   Updated: 2025/04/03 21:09:21 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Crée un nouveau noeud heredoc à partir d'un token.
t_file	*create_new_heredoc_node(t_token *token)
{
	t_file	*new_node;

	new_node = ft_malloc(sizeof(t_file));
	if (new_node == NULL)
		return (NULL);
	new_node->mode = HEREDOC;
	new_node->path = ft_strdup(token->content);
	new_node->next = NULL;
	return (new_node);
}

// Ajoute ou remplace un heredoc dans la liste selon le token fourni.
t_file	*add_or_replace_heredoc(t_file *files, t_token *heredoc_token)
{
	t_file	*found;
	t_file	*last;
	t_file	*new_node;

	found = find_existing_heredoc(files);
	if (found != NULL)
	{
		found->path = ft_strdup(heredoc_token->content);
		return (files);
	}
	new_node = create_new_heredoc_node(heredoc_token);
	if (new_node == NULL)
		return (files);
	if (files == NULL)
		return (new_node);
	last = find_last_node(files);
	if (last != NULL)
		last->next = new_node;
	return (files);
}
