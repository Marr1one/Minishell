/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:02:12 by maissat           #+#    #+#             */
/*   Updated: 2025/04/07 19:50:46 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*process_heredoc(t_data *data, t_file *file, int current_index,
		int last_index)
{
	char	*content;

	content = read_heredoc(data, file->path, "> ");
	if (current_index != last_index)
		return (NULL);
	return (content);
}

static int	heredoc_should_abort(void)
{
	return (g_signalhook != 0);
}

static char	*handle_heredoc_content(t_data *data, t_file *file,
		int current_index, int last_index)
{
	char	*temp;

	if (heredoc_should_abort())
		return (NULL);
	temp = process_heredoc(data, file, current_index, last_index);
	if (heredoc_should_abort())
	{
		free(temp);
		return (NULL);
	}
	if (current_index == last_index)
		return (temp);
	free(temp);
	return (NULL);
}

static char	*iterate_heredocs(t_data *data, t_file *files, int last_index)
{
	t_file	*current;
	char	*content;
	char	*temp;
	int		i;

	i = 0;
	content = NULL;
	current = files;
	while (current)
	{
		if (current->mode == HEREDOC)
		{
			temp = handle_heredoc_content(data, current, i, last_index);
			if (heredoc_should_abort())
				return (NULL);
			if (temp)
				content = temp;
		}
		current = current->next;
		i++;
	}
	return (content);
}

char	*execute_heredocs(t_data *data, t_file *files, int last_index)
{
	g_signalhook = 0;
	return (iterate_heredocs(data, files, last_index));
}
