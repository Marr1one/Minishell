/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:07:42 by maissat           #+#    #+#             */
/*   Updated: 2025/04/07 19:27:34 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_heredoc_end(char *line, char *delimiter)
{
	if (!line)
	{
		printf("minishell: warning: here-document"
			"delimited by EOF (wanted `%s`)\n",
			delimiter);
		return (1);
	}
	if (ft_strlcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

// Lit via readline jusqu'au délimiteur et retourne le contenu lu.
char	*heredoc_loop(t_data *data, char *delimiter, char *prompt)
{
	char	*content;
	char	*line;

	(void)data;
	content = ft_strdup("");
	setup_signals_heredoc();
	while (1)
	{
		line = readline(prompt);
		if (g_signalhook != 0)
			return (free(line), NULL);
		if (is_heredoc_end(line, delimiter))
			break ;
		content = ft_strjoin(content, line);
		content = ft_strjoin(content, "\n");
		free(line);
	}
	return (content);
}

// lit le heredoc
char	*read_heredoc(t_data *data, char *delimiter, char *prompt)
{
	char	*content;

	content = heredoc_loop(data, delimiter, prompt);
	return (content);
}
