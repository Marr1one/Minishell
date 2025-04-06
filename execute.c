/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:15:55 by maissat           #+#    #+#             */
/*   Updated: 2025/04/06 17:58:22 by braugust         ###   ########.fr       */
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

void execute_child_process(t_data *data, t_cmd *cmd, int fd_in, int *fd_pipe)
{
    // Configuration des signaux pour l'enfant
    setup_child_signals();
    
    // Rediriger les entrées/sorties seulement si les descripteurs sont valides
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
    
    // Fermer tous les descripteurs de pipe dans l'enfant
    if (fd_pipe[0] >= 0)
        close(fd_pipe[0]);
    if (fd_pipe[1] >= 0)
        close(fd_pipe[1]);
    
    // Gérer les redirections de fichiers et les heredocs
    handle_heredoc_and_files(cmd);
    
    // Exécuter la commande
    execute_command(data, cmd);
    
    // Si execute_command retourne, c'est une erreur
    exit(127);  // Commande non trouvée
}

void execute_cmds(t_data *data, t_cmd *cmds)
{
    int fd_in;
    int fd_pipe[2] = {-1, -1};  // Initialiser à -1 pour indiquer descripteurs invalides
    t_cmd *cmd;
    pid_t *pids;
    int i;
    int nb_cmd;
    
    // Si heredoc échoue, retourner
    if (handle_all_heredocs(data, cmds) == 1)
        return;
    
    // Compter le nombre de commandes
    nb_cmd = count_cmds(cmds);
    
    // Vérifier si c'est un builtin à exécuter dans le shell parent
    if (check_single_builtin(data, cmds))
        return;
    
    // Allocation pour stocker les PIDs
    pids = ft_malloc(sizeof(pid_t) * nb_cmd);
    if (!pids)
        return;
    
    // Initialisation
    fd_in = 0;  // 0 = stdin standard
    i = 0;
    cmd = cmds;
    
    // Exécution de chaque commande
    while (cmd)
    {
        // Créer un pipe pour la commande suivante si nécessaire
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
            // Dernière commande, pas de pipe de sortie
            fd_pipe[0] = -1;  
            fd_pipe[1] = -1;
        }
        
        // Créer un processus enfant
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork");
            // Nettoyer les descripteurs
            if (fd_in > 0)
                close(fd_in);
            if (fd_pipe[0] >= 0)
                close(fd_pipe[0]);
            if (fd_pipe[1] >= 0)
                close(fd_pipe[1]);
            free(pids);
            return;
        }
        
        if (pids[i] == 0)
        {
            // Processus enfant
            execute_child_process(data, cmd, fd_in, fd_pipe);
            // Ne devrait jamais arriver ici
            exit(127);
        }
        else
        {
            // Processus parent
            // Fermer les descripteurs qui ne sont plus nécessaires
            if (fd_in > 0)  // Ne pas fermer stdin standard (0)
                close(fd_in);
                
            if (cmd->next)
            {
                // Fermer l'extrémité d'écriture du pipe
                close(fd_pipe[1]);
                // Conserver l'extrémité de lecture pour la prochaine commande
                fd_in = fd_pipe[0];
            }
        }
        
        i++;
        cmd = cmd->next;
    }
    
    // Attendre tous les processus enfants
    for (int j = 0; j < nb_cmd; j++)
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
    }
    
    free(pids);
}