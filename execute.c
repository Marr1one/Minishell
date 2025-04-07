/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:15:55 by maissat           #+#    #+#             */
/*   Updated: 2025/04/07 19:27:00 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	init_cmd_execution(t_data *data, t_cmd *cmds, pid_t **pids, int *nb_cmd)
{
	// Gestion des heredocs
	if (handle_all_heredocs(data, cmds) == 1)
		return (0);
	// Vérification des builtins simples
	*nb_cmd = count_cmds(cmds);
	if (check_single_builtin(data, cmds))
		return (0);
	// Allocation de mémoire pour les PIDs
	*pids = ft_malloc(sizeof(pid_t) * (*nb_cmd));
	if (!*pids)
		return (0);
	return (1);
}

int	setup_cmd_pipe(int fd_pipe[2], t_cmd *cmd)
{
	if (cmd->next)
	{
		if (pipe(fd_pipe) == -1)
		{
			perror("pipe");
			return (0);
		}
	}
	else
	{
		fd_pipe[0] = -1;
		fd_pipe[1] = -1;
	}
	return (1);
}

// Gestion des erreurs de fork et nettoyage
void	handle_fork_error(pid_t *pids, int fd_in, int fd_pipe[2])
{
	perror("fork");
	if (fd_in > 0)
		close(fd_in);
	if (fd_pipe[0] >= 0)
		close(fd_pipe[0]);
	if (fd_pipe[1] >= 0)
		close(fd_pipe[1]);
	free(pids);
}

// Gestion des descripteurs de fichiers dans le processus parent
int	handle_parent_descriptors(int *fd_in, int fd_pipe[2], t_cmd *cmd)
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

// Attente et gestion des statuts des processus enfants
void	wait_for_children(t_data *data, pid_t *pids, int nb_cmd)
{
	int	j;
	int	status;

	j = 0;
	while (j < nb_cmd)
	{
		waitpid(pids[j], &status, 0);
		// Gestion du statut du dernier processus
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
	free(pids);
}

// Fonction principale d'exécution des commandes
void	execute_cmds(t_data *data, t_cmd *cmds)
{
	int fd_in;
	int fd_pipe[2];
	t_cmd *cmd;
	pid_t *pids;
	int i;
	int nb_cmd;

	if (!init_cmd_execution(data, cmds, &pids, &nb_cmd))
		return ;
	fd_in = 0;
	i = 0;
	cmd = cmds;
	while (cmd)
	{
		if (!setup_cmd_pipe(fd_pipe, cmd))
		{
			free(pids);
			return ;
		}
		pids[i] = fork();
		if (pids[i] == -1)
		{
			handle_fork_error(pids, fd_in, fd_pipe);
			return ;
		}
		if (pids[i] == 0)
			execute_child_process(data, cmd, fd_in, fd_pipe);
		else
			handle_parent_descriptors(&fd_in, fd_pipe, cmd);
		i++;
		cmd = cmd->next;
	}
	wait_for_children(data, pids, nb_cmd);
}