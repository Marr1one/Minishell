/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:07:42 by maissat           #+#    #+#             */
/*   Updated: 2025/04/07 19:30:53 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*init_heredoc_loop(void)
{
	setup_signals_heredoc();
	return (ft_strdup(""));
}

char	*heredoc_loop(t_data *data, char *delimiter, char *prompt)
{
	char	*content;
	char	*line;
	
	(void)data;
	content = init_heredoc_loop();
	while (1)
	{
		line = readline(prompt);
		if (!line)
			break ;
		if (variable_globale != 0)
			return (NULL);
		if (ft_strlcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		content = ft_strjoin(content, line);
		content = ft_strjoin(content, "\n");
		free(line);
	}
	return (content);
}

char	*read_heredoc(t_data *data, char *delimiter, char *prompt)
{
	char	*content;

	content = heredoc_loop(data, delimiter, prompt);
	return (content);
}
