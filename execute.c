/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:15:55 by maissat           #+#    #+#             */
/*   Updated: 2025/03/27 03:37:41 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*new_test_commands(char **paths, char *str)
{
	int		i;
	char	*path_test;
	
	if (!paths)
		return (NULL);
	if (!*str)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path_test = ft_join(paths[i], str);
		if (access(path_test, F_OK | X_OK) == 0)
			return (path_test);
		i++;
	}
	return (NULL);
}

int open_file(char *path, t_type mode)
{
    int fd = -1;
    if (mode == INFILE)
        fd = open(path, O_RDONLY);
    else if (mode == OUTFILE_TRUNC)
        fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (mode == OUTFILE_APPEND)
        fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (mode == HEREDOC)
        fd = heredoc_input(path);
    return fd;
}

int execute_builtin_child(t_cmd *cmd, t_data *data)
{
	int	i;
	t_cmd *current_cmd;
	
	current_cmd = cmd;
	if (ft_strlcmp(current_cmd->args[0], "unset") == 0)
    {
		if (current_cmd->args[1])
		{
			i =  1;
			while (current_cmd->args[i])
				check_unset(data, current_cmd->args[i++]);
		}
        return (1);
    }
    if (ft_strlcmp(cmd->args[0], "echo") == 0)
    {
        ft_echo(cmd);
        return (1);
    }
	if (ft_strlcmp(cmd->args[0], "export") == 0)
	{
		ft_export(cmd, data);
		return (1);
	}
	if (ft_strlcmp(cmd->args[0], "cd") == 0)
	{
		ft_cd(cmd);
		return (1);
	}
	if (ft_strlcmp(cmd->args[0], "env") == 0)
	{
		show_env(data->envp);
		return (1);	
	}
	else if (ft_strlcmp(cmd->args[0], "exit") == 0)
	{
		ft_exit(cmd);
		return (1);
	}
    else if (ft_strlcmp(cmd->args[0], "pwd") == 0)
    {
        ft_pwd(cmd);
        return (1);
    }
    return (0);
}

int ncheck_builtin(t_cmd *cmd)
{
	if (ft_strlcmp(cmd->args[0], "echo") == 0)
        return (1);
	if (ft_strlcmp(cmd->args[0], "unset") == 0)
        return (1);
	if (ft_strlcmp(cmd->args[0], "export") == 0)
		return (1);
	if (ft_strlcmp(cmd->args[0], "cd") == 0)
		return (1);
	if (ft_strlcmp(cmd->args[0], "env") == 0)
		return (1);	
	else if (ft_strlcmp(cmd->args[0], "exit") == 0)
		return (1);
    else if (ft_strlcmp(cmd->args[0], "pwd") == 0)
        return (1);
    return (0);
}

int execute_builtin0(t_cmd *cmd, t_data *data)
{
	int	i;
	t_cmd *current_cmd;
	
	current_cmd = cmd;
	if (ft_strlcmp(current_cmd->args[0], "unset") == 0)
    {
		if (current_cmd->args[1])
		{
			i =  1;
			while (current_cmd->args[i])
				check_unset(data, current_cmd->args[i++]);
		}
        return (1);
    }
	if (ft_strlcmp(cmd->args[0], "export") == 0)
	{
		ft_export(cmd, data);
		return (1);
	}
	if (ft_strlcmp(cmd->args[0], "cd") == 0)
	{
		ft_cd(cmd);
		return (1);
	}
	if (ft_strlcmp(cmd->args[0], "env") == 0)
	{
		show_env(data->envp);
		return (1);	
	}
	else if (ft_strlcmp(cmd->args[0], "exit") == 0)
	{
		ft_exit(cmd);
		return (1);
	}
    else if (ft_strlcmp(cmd->args[0], "pwd") == 0)
    {
        ft_pwd(cmd);
        return (1);
    }
    return (0);
}

//Mini_shell$ pwd
///home/maissat/Desktop/minishellm21/built_in			
//Mini_shell$ pwd | cd ..
//Mini_shell$ pwd
///home/maissat/Desktop/minishellm21

int	count_cmds(t_cmd *list_cmds)
{
	int		count;
	t_cmd	*current_cmd;

	current_cmd = list_cmds;
	count = 0;
	while (current_cmd)
	{
		count++;
		current_cmd = current_cmd->next;
	}
	return (count);
}

// void	execute_cmds(t_data *data, t_cmd *cmds)
// {
//     int     fd_in = 0;
//     int     fd_pipe[2];
//     int     fd;
// 	char	**paths;
//     char    *good_path;
//     pid_t   pid;
//     t_file  *current_file;
//     t_cmd   *current_cmd;
// 	int stdin_backup; 
// 	int stdout_backup;
	
// 	current_cmd = cmds;
//     while (current_cmd)
//     {
//         if (current_cmd->next)
//             pipe(fd_pipe);
// 		else if (count_cmds(cmds) == 1 && ncheck_builtin(current_cmd) == 1)
// 		{
// 			if (current_cmd->files)
//             {
// 				stdin_backup = dup(STDIN_FILENO);
//    				stdout_backup = dup(STDOUT_FILENO);
//                 current_file = current_cmd->files;
//                 while (current_file)
//                 {

//                     fd = open_file(current_file->path, current_file->mode);
//                     if (fd == -1)
//                     {
//                         perror("open");
//                         exit(1); // free and exit
//                     }
			
//                     if (current_file->mode == INFILE || current_file->mode == HEREDOC)
//                         dup2(fd, STDIN_FILENO);
//                     else
// 					{
//                         dup2(fd, STDOUT_FILENO);
// 					}
//                     close(fd);

//                     current_file = current_file->next;
//                 }
//             }
// 			execute_builtin_child(current_cmd, data);
// 			dup2(stdin_backup, STDIN_FILENO);
// 			dup2(stdout_backup, STDOUT_FILENO);
// 			close(stdin_backup);
// 			close(stdout_backup);
// 			return ;
// 		}
	
//         pid = fork();
//         if (pid == 0)
//         {
//             if (current_cmd->next)
//             {
//                 dup2(fd_pipe[1], STDOUT_FILENO);
//                 close(fd_pipe[0]);
//                 close(fd_pipe[1]);
//             }
//             if (fd_in != 0)
//             {
//                 dup2(fd_in, STDIN_FILENO);
//                 close(fd_in);
//             }
//             if (current_cmd->files)
//             {
// 				printf("on rentre bien dans le cas files\n");
//                 current_file = current_cmd->files;
//                 while (current_file)
//                 {
//                     fd = open_file(current_file->path, current_file->mode);
//                     if (fd == -1)
//                     {
//                         perror("open");
//                         exit(1); // free and exit
//                     }	
//                     if (current_file->mode == INFILE || current_file->mode == HEREDOC)
//                         dup2(fd, STDIN_FILENO);
//                     else
//                         dup2(fd, STDOUT_FILENO);
//                     close(fd);
//                     current_file = current_file->next;
//                 }
//             }
//           	if (execute_builtin_child(current_cmd, data) == 1)
//                 exit(0);
// 			paths = ft_split(get_path_env(data->envp), ':');
// 			paths = add_slash_all(paths);
//             good_path = new_test_commands(paths, current_cmd->args[0]);
//             if (good_path != NULL)
//             {
				
//                 execve(good_path, current_cmd->args, data->envp);
//  				perror("execve");
//                 exit(data->exit_status);
//             }
//             else
//             {
//                 printf("minishell: %s: command not found\n", current_cmd->args[0]);
//                 exit(127);
//             }
//         }
//         else if (pid > 0)
//         {
//             waitpid(pid, NULL, 0);
//             if (fd_in != 0)
//                 close(fd_in);
//             if (current_cmd->next)
//             {
//                 close(fd_pipe[1]);
//                 fd_in = fd_pipe[0];
//             }
//         }
//         current_cmd = current_cmd->next;
//     }
// }

void handle_file_redirections(t_cmd *current_cmd)
{
    t_file *current_file;
    int fd;

    if (!current_cmd->files)
        return;

    current_file = current_cmd->files;
    while (current_file)
    {
        fd = open_file(current_file->path, current_file->mode);
        if (fd == -1)
        {
            perror("open");
            exit(1);
        }

        if (current_file->mode == INFILE || current_file->mode == HEREDOC)
            dup2(fd, STDIN_FILENO);
        else
            dup2(fd, STDOUT_FILENO);

        close(fd);
        current_file = current_file->next;
    }
}

/* Gère l'exécution des built-ins à part */
void handle_single_builtin(t_data *data, t_cmd *current_cmd)
{
    int stdin_backup;
    int stdout_backup;

    stdin_backup = dup(STDIN_FILENO);
    stdout_backup = dup(STDOUT_FILENO);

    handle_file_redirections(current_cmd);
    execute_builtin_child(current_cmd, data);

    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdin_backup);
    close(stdout_backup);

}

void execute_command_path(t_data *data, char **paths, t_cmd *current_cmd)
{
    char *good_path;

    good_path = new_test_commands(paths, current_cmd->args[0]);
    if (good_path != NULL)
    {
        execve(good_path, current_cmd->args, data->envp);
        perror("execve");
        exit(data->exit_status);
    }
    else
    {
        printf("minishell: %s: command not found\n", current_cmd->args[0]);
        exit(127);
    }
}

/* Prépare et exécute une commande dans un processus enfant */
void execute_child_process(t_data *data, t_cmd *current_cmd, int fd_in, int *fd_pipe)
{
    char **paths;

    if (current_cmd->next)
    {
        dup2(fd_pipe[1], STDOUT_FILENO);
        close(fd_pipe[0]);
        close(fd_pipe[1]);
    }
    if (fd_in != 0)
    {
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }
    handle_file_redirections(current_cmd);
    if (execute_builtin_child(current_cmd, data) == 1)
        exit(0);
    paths = ft_split(get_path_env(data->envp), ':');
    paths = add_slash_all(paths);
	execute_command_path(data, paths, current_cmd);
}

void handle_parent_process(int *fd_in, int *fd_pipe, t_cmd *current_cmd)
{
    waitpid(0, NULL, 0);
    if (*fd_in != 0)
        close(*fd_in);
    
    if (current_cmd->next)
    {
        close(fd_pipe[1]);
        *fd_in = fd_pipe[0];
    }
}

/* Fonction principale de gestion des commandes */
void execute_cmds(t_data *data, t_cmd *cmds)
{
    int fd_in = 0;
    int fd_pipe[2];
    pid_t pid;
    t_cmd *current_cmd;

    current_cmd = cmds;
    while (current_cmd)
    {
        if (current_cmd->next)
            pipe(fd_pipe);
        else if (count_cmds(cmds) == 1 && ncheck_builtin(current_cmd) == 1)
        {
            handle_single_builtin(data, current_cmd);
            return;
        }
        pid = fork();
        if (pid == 0)
            execute_child_process(data, current_cmd, fd_in, fd_pipe);
        else if (pid > 0)
			handle_parent_process(&fd_in, fd_pipe, current_cmd);
        current_cmd = current_cmd->next;
    }
}
