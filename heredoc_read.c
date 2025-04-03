/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:07:42 by maissat           #+#    #+#             */
/*   Updated: 2025/04/03 21:09:26 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Lit via readline jusqu'au délimiteur et retourne le contenu lu.
char	*heredoc_loop(char *delimiter, char *prompt)
{
	char	*content;
	char	*line;
	char	*tmp;

	content = ft_strdup("");
	line = readline(prompt);
	while (line != NULL)
	{
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
		line = readline(prompt);
	}
	return (content);
}

// Ouvre le tty et lit le heredoc en redirigeant temporairement STDIN.
char	*read_heredoc_from_tty(char *delimiter, char *prompt)
{
	char	*content;
	int		tty_fd;
	int		std_backup;

	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd == -1)
	{
		close(tty_fd);
		printf("open /dev/tty");
		return (ft_strdup(""));
	}
	std_backup = dup(STDIN_FILENO);
	close(std_backup);
	dup2(tty_fd, STDIN_FILENO);
	close(tty_fd);
	content = heredoc_loop(delimiter, prompt);
	dup2(std_backup, STDIN_FILENO);
	close(std_backup);
	return (content);
}
