/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 04:05:59 by maissat           #+#    #+#             */
/*   Updated: 2025/04/08 15:20:07 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_fork_error(int fd_in, int *fd_pipe)
{
	perror("fork");
	if (fd_in > 0)
		close(fd_in);
	if (fd_pipe[0] >= 0)
		close(fd_pipe[0]);
	if (fd_pipe[1] >= 0)
		close(fd_pipe[1]);
}

int	handle_pid_error(pid_t *pids, int i, int fd_in, int *fd_pipe)
{
	if (pids[i] == -1)
	{
		handle_fork_error(fd_in, fd_pipe);
		return (1);
	}
	return (0);
}
