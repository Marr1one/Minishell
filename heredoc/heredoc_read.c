/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:07:42 by maissat           #+#    #+#             */
/*   Updated: 2025/04/05 15:47:00 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Lit via readline jusqu'au délimiteur et retourne le contenu lu.
char	*heredoc_loop(t_data *data, char *delimiter, char *prompt)
{
	char	*content;
	char	*line;
	char	*tmp;
	
	content = ft_strdup("");
	setup_signals_heredoc();
	while (1)
	{
		line = readline(prompt);
		if (!line) // Vérifie si heredoc a été annulé
			break ;
		if (variable_globale != 0)
		{
			data->exit_status = 128 + variable_globale;
			return (variable_globale = 0, NULL);
		}
		if (ft_strlcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		tmp = content;
		content = ft_strjoin(content, line);
		tmp = content;
		content = ft_strjoin(content, "\n");
		free(line);
	}
	return (content);
}

// Ouvre le tty et lit le heredoc en redirigeant temporairement STDIN.
char	*read_heredoc_from_tty(t_data *data, char *delimiter, char *prompt)
{
	char	*content;

	content = heredoc_loop(data, delimiter, prompt);
	return (content);
}
