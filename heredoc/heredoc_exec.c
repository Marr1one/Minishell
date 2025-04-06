/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:02:12 by maissat           #+#    #+#             */
/*   Updated: 2025/04/06 17:31:25 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*process_heredoc(t_data *data, t_file *file, int current_index, int last_index)
{
	char	*content;

	content = read_heredoc_from_tty(data, file->path, "> ");
	if (current_index != last_index)
		return (NULL);
	return (content);
}

// char	*execute_heredocs(t_data *data, t_file *files, int last_index)
// {
// 	t_file	*current;
// 	char	*content;
// 	int		current_index;
// 	char	*temp_content;

// 	current = files;
// 	current_index = 0;
// 	content = NULL;
// 	while (current)
// 	{
// 		if (current->mode == HEREDOC)
// 		{
// 			temp_content = process_heredoc(data, current, current_index, last_index);
// 			if (temp_content == NULL && data->exit_status != 0)
// 				return (NULL);
// 			if (current_index == last_index)
// 				content = temp_content;
// 			else
// 				free(temp_content);
// 		}
// 		current = current->next;
// 		current_index++;
// 	}
// 	return (content);
// }

char	*execute_heredocs(t_data *data, t_file *files, int last_index)
{
	t_file	*current;
	char	*content = NULL;
	char	*temp_content;
	int		current_index = 0;

	variable_globale = 0; // reset avant de commencer

	current = files;
	while (current)
	{
		if (current->mode == HEREDOC)
		{
			if (variable_globale != 0)
			{
				variable_globale = 0;
				return (NULL);
			}
			temp_content = process_heredoc(data, current, current_index, last_index);
			if (variable_globale != 0)
			{
				variable_globale = 0;
				free(temp_content);
				return (NULL);
			}
			if (current_index == last_index)
				content = temp_content;
			else
				free(temp_content);
		}
		current = current->next;
		current_index++;
	}
	return (content);
}
