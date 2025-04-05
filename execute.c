/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:15:55 by maissat           #+#    #+#             */
/*   Updated: 2025/04/05 13:29:10 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_pipes(t_cmd *current_cmd, int fd_in, int *fd_pipe)
{
	if (current_cmd->next)
	{
		dup2(fd_pipe[1], STDOUT_FILENO);
		close(fd_pipe[0]);
		close(fd_pipe[1]);
	}
	if (fd_in != 0 && !current_cmd->heredoc_content)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
}

void	handle_parent_process(t_data *data, int *fd_in, int *fd_pipe,
		t_cmd *current_cmd)
{
	int	status;

	waitpid(0, &status, 0);
	if (WIFEXITED(status))
	{
		data->exit_status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			printf("Quit (core dumped)\n");
		}
		data->exit_status = 128 + WTERMSIG(status);
	}
	if (*fd_in != 0)
		close(*fd_in);
	if (current_cmd->next)
	{
		close(fd_pipe[1]);
		*fd_in = fd_pipe[0];
	}
}

void	execute_forked_cmd(t_data *data, t_cmd *cmd, int *fd_in, int *fd_pipe)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		execute_child_process(data, cmd, *fd_in, fd_pipe);
	}
	else if (pid > 0)
		handle_parent_process(data, fd_in, fd_pipe, cmd);
}

void	execute_cmds(t_data *data, t_cmd *cmds)
{
	int		fd_in;
	int		fd_pipe[2];
	t_cmd	*cmd;

	if (handle_all_heredocs(data, cmds) == 1)
		return ;
	fd_in = 0;
	cmd = cmds;
	if (check_single_builtin(data, cmds))
		return ;
	while (cmd)
	{
		setup_pipe(cmd, fd_pipe);
		execute_forked_cmd(data, cmd, &fd_in, fd_pipe);
		cmd = cmd->next;
	}
}
