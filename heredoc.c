/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 08:11:37 by braugust          #+#    #+#             */
/*   Updated: 2025/03/03 09:25:15 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int heredoc_input(char *delimiter)
{
    int pipefd[2];
    char *line;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        // Si la ligne correspond exactement au délimiteur on stop
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(pipefd[1], line, ft_strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    // On ferme le côté écriture car seul le côté lecture sera utilisé
    close(pipefd[1]);
    return (pipefd[0]);
}