/* ************************************************************************** */
/*									                                        */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 05:13:51 by braugust          #+#    #+#             */
/*   Updated: 2025/04/03 20:28:23 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heredoc(t_cmd *current_cmd)
{
	int		heredoc_pipe[2];
	char	*heredoc_content;

	if (contains_heredoc(current_cmd))
	{
		heredoc_content = execute_last_heredoc(current_cmd);
		if (heredoc_content != NULL)
		{
			if (pipe(heredoc_pipe) == -1)
			{
				perror("pipe");
				exit(1);
			}
			write(heredoc_pipe[1], heredoc_content, ft_strlen(heredoc_content));
			close(heredoc_pipe[1]);
			dup2(heredoc_pipe[0], STDIN_FILENO);
			close(heredoc_pipe[0]);
		}
	}
}

// Vérifie si une commande contient au moins un heredoc.
int	contains_heredoc(t_cmd *cmd)
{
	t_file	*current;

	current = cmd->files;
	while (current)
	{
		if (current->mode == HEREDOC)
			return (1);
		current = current->next;
	}
	return (0);
}

// Exécute le heredoc en récupérant le contenu du dernier 
// heredoc de la commande.
char	*execute_last_heredoc(t_cmd *cmd)
{
	t_file	*last;
	int		last_index;
	char	*content;

	last = find_last_heredoc(cmd->files, &last_index);
	if (last == NULL)
		return (NULL);
	content = execute_heredocs(cmd->files, last_index);
	return (content);
}
