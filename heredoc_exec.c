/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:02:12 by maissat           #+#    #+#             */
/*   Updated: 2025/04/04 14:54:31 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_heredoc(t_file *file, int current_index, int last_index)
{
	char	*content;

	content = read_heredoc_from_tty(file->path, "> ");
	if (current_index != last_index)
		return (NULL);
	return (content);
}

char	*execute_heredocs(t_file *files, int last_index)
{
	t_file	*current;
	char	*content;
	int		current_index;
	char	*temp_content;

	current = files;
	current_index = 0;
	content = NULL;
	while (current)
	{
		if (current->mode == HEREDOC)
		{
			temp_content = process_heredoc(current, current_index, last_index);
			if (temp_content != NULL)
				content = temp_content;
		}
		current = current->next;
		current_index++;
	}
	return (content);
}
