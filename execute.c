/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:15:55 by maissat           #+#    #+#             */
/*   Updated: 2025/04/06 17:57:57 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//void	redirect_pipes(t_cmd *current_cmd, int fd_in, int *fd_pipe)
//{
//	if (current_cmd->next)
//	{
//		dup2(fd_pipe[1], STDOUT_FILENO);
//		close(fd_pipe[0]);
//		close(fd_pipe[1]);
//	}
//	if (fd_in != 0)
//	{
//		dup2(fd_in, STDIN_FILENO);
//		close(fd_in);
//	}
//}
//void redirect_pipes(t_cmd *current_cmd, int fd_in, int *fd_pipe)
//{
//    // Redirection de l'entrée standard si nécessaire
//    if (fd_in != 0)
//    {
//        if (dup2(fd_in, STDIN_FILENO) == -1)
//            perror("dup2 stdin");
//        close(fd_in);
//    }
    
//    // Redirection de la sortie standard si une commande suit
//    if (current_cmd->next)
//    {
//        if (dup2(fd_pipe[1], STDOUT_FILENO) == -1)
//            perror("dup2 stdout");
//    }
    
//    // Fermer tous les descripteurs de pipe qui ne sont plus nécessaires
//    if (current_cmd->next)
//    {
//        close(fd_pipe[0]);
//        close(fd_pipe[1]);
//    }
//}
//void	handle_parent_process(t_data *data, t_cmd *current_cmd)
//{
//	int	status;

//	//waitpid(0, &status, 0);
//	if (WIFEXITED(status))
//	{
//		data->exit_status = WEXITSTATUS(status);
//	}
//	else if (WIFSIGNALED(status))
//	{
//		if (WTERMSIG(status) == SIGQUIT)
//		{
//			printf("Quit (core dumped)\n");
//		}
//		data->exit_status = 128 + WTERMSIG(status);
//	}
//	//if (*fd_in != 0)
//	//	close(*fd_in);
//	//if (current_cmd->next)
//	//{
//	//	close(fd_pipe[1]);
//	//	*fd_in = fd_pipe[0];
//	//}
//}

void	execute_child_process(t_data *data, t_cmd *cmd, int fd_in, int *fd_pipe)
{
	setup_child_signals();
	if (fd_in > 0)  // Seulement si fd_in est un descripteur valide (>0)
	{
		if (dup2(fd_in, STDIN_FILENO) == -1)
			perror("dup2 stdin");
		close(fd_in);
	}
	if (cmd->next && fd_pipe[1] >= 0)  // Vérifier que fd_pipe[1] est valide
	{
		if (dup2(fd_pipe[1], STDOUT_FILENO) == -1)
			perror("dup2 stdout");
	}
	if (fd_pipe[0] >= 0)
		close(fd_pipe[0]);
	if (fd_pipe[1] >= 0)
		close(fd_pipe[1]);

	handle_heredoc_and_files(cmd);
	execute_command(data, cmd);
	}

void execute_cmds(t_data *data, t_cmd *cmds)
{
	int fd_in;
	int fd_pipe[2];
	t_cmd *cmd;
	pid_t *pids;
	int i;
	int nb_cmd;
	int	j;

	if (handle_all_heredocs(data, cmds) == 1)
		return;
	nb_cmd = count_cmds(cmds);
	if (check_single_builtin(data, cmds))
		return;
	pids = ft_malloc(sizeof(pid_t) * nb_cmd);
	if (!pids)
		return;
	fd_in = 0;  // 0 = stdin standard
	i = 0;
	cmd = cmds;
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(fd_pipe) == -1)
			{
				perror("pipe");
				free(pids);
				return;
			}
		}
		else
		{
			fd_pipe[0] = -1;  
			fd_pipe[1] = -1;
		}
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			if (fd_in > 0)
				close(fd_in);
			if (fd_pipe[0] >= 0)				//erreur de fork
				close(fd_pipe[0]);
			if (fd_pipe[1] >= 0)
				close(fd_pipe[1]);
			free(pids);
			return;
		}
		if (pids[i] == 0)
			execute_child_process(data, cmd, fd_in, fd_pipe);
		else
		{
			// Processus parent
			// Fermer les descripteurs qui ne sont plus nécessaires
			if (fd_in > 0)  // Ne pas fermer stdin standard (0)
				close(fd_in);
			if (cmd->next)
			{
				close(fd_pipe[1]);
				// Conserver l'extrémité de lecture pour la prochaine commande
				fd_in = fd_pipe[0];
			}
		}
		i++;
		cmd = cmd->next;
	}
	j = 0;
	// Attendre tous les processus enfants
	while (j < nb_cmd)
	{
		int status;
		waitpid(pids[j], &status, 0);
		
		// Le statut de sortie est celui du dernier processus
		if (j == nb_cmd - 1)
		{
			if (WIFEXITED(status))
				data->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGQUIT)
					printf("Quit (core dumped)\n");
				data->exit_status = 128 + WTERMSIG(status);
			}
		}
		j++;
	}
}