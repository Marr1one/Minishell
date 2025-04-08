/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_parent.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 04:08:25 by maissat           #+#    #+#             */
/*   Updated: 2025/04/08 15:19:26 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_parent_descriptors(int *fd_in, int *fd_pipe, t_cmd *cmd)
{
	if (*fd_in > 0)
		close(*fd_in);
	if (cmd->next)
	{
		close(fd_pipe[1]);
		*fd_in = fd_pipe[0];
	}
	return (1);
}

void	wait_for_children(t_cmd *cmds, t_data *data, pid_t *pids)
{
	int	j;
	int	status;
	int	nb_cmd;

	j = 0;
	nb_cmd = count_cmds(cmds);
	while (j < nb_cmd)
	{
		waitpid(pids[j], &status, 0);
		if (j == nb_cmd - 1)
		{
			if (WIFEXITED(status))
				data->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGINT)
					printf("\n");
				if (WTERMSIG(status) == SIGQUIT)
					printf("Quit (core dumped)\n");
				data->exit_status = 128 + WTERMSIG(status);
			}
		}
		j++;
	}
}
