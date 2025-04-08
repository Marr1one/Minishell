/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:15:55 by maissat           #+#    #+#             */
/*   Updated: 2025/04/09 01:11:27 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h" 

void	execute_child_process(t_data *data, t_cmd *cmd, int fd_in, int *fd_pipe)
{
	setup_child_signals();
	if (fd_in > 0)
	{
		if (dup2(fd_in, STDIN_FILENO) == -1)
			perror("dup2 stdin");
		close(fd_in);
	}
	if (cmd->next && fd_pipe[1] >= 0)
	{
		if (dup2(fd_pipe[1], STDOUT_FILENO) == -1)
			perror("dup2 stdout");
		close(fd_pipe[1]);
	}
	if (fd_pipe[0] >= 0)
		close(fd_pipe[0]);
	if (fd_pipe[1] >= 0)
		close(fd_pipe[1]);
	handle_heredoc_and_files(cmd);
	execute_command(data, cmd);
}

int	init_cmd_execution(t_data *data, t_cmd *cmds, pid_t **pids)
{
	t_cmd	*curr_cmd;
	int		i;

	i = 0;
	curr_cmd = cmds;
	if (handle_all_heredocs(data, cmds) == 1)
		return (0);
	if (check_single_builtin(data, cmds))
		return (0);
	while (curr_cmd)
	{
		i++;
		curr_cmd = curr_cmd->next;
	}
	*pids = ft_malloc(sizeof(pid_t) * (i));
	if (!*pids)
		return (0);
	return (1);
}

void	execute_cmds(t_data *data, t_cmd *cmds)
{
	int		fd_in;
	int		fd_pipe[2];
	t_cmd	*cmd;
	pid_t	*pids;
	int		i;

	if (!init_cmd_execution(data, cmds, &pids))
		return ;
	fd_in = 0;
	i = -1;
	cmd = cmds;
	while (cmd)
	{
		if (!setup_cmd_pipe(fd_pipe, cmd))
			return ;
		pids[++i] = fork();
		if (handle_pid_error(pids, i, fd_in, fd_pipe) == 1)
			return ;
		if (pids[i] == 0)
			execute_child_process(data, cmd, fd_in, fd_pipe);
		else
			handle_parent_descriptors(&fd_in, fd_pipe, cmd);
		cmd = cmd->next;
	}
	wait_for_children(cmds, data, pids);
}
