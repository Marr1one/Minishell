/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:07:12 by braugust          #+#    #+#             */
/*   Updated: 2025/04/05 13:26:06 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

void	setup_heredoc(t_cmd *cmd)
{
	int	heredoc_pipe[2];

	if (pipe(heredoc_pipe) == -1)
	{
		perror("pipe");
		exit(1);
	}
	write(heredoc_pipe[1], cmd->heredoc_content,
		ft_strlen(cmd->heredoc_content));
	close(heredoc_pipe[1]);
	dup2(heredoc_pipe[0], STDIN_FILENO);
	close(heredoc_pipe[0]);
}

void	handle_heredoc_and_files(t_cmd *cmd)
{
	handle_file_redirections(cmd);
	if (cmd->heredoc_content)
		setup_heredoc(cmd);
}

int	handle_all_heredocs(t_data *data, t_cmd *cmds)
{
	t_cmd	*cmd;

	cmd = cmds;
	while (cmd)
	{
		if (contains_heredoc(cmd))
		{
			cmd->heredoc_content = execute_last_heredoc(data, cmd);
			if (cmd->heredoc_content == NULL)
				return 1;
		}
		cmd = cmd->next;
	}
	return (0);
}
