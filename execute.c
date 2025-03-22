/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:15:55 by maissat           #+#    #+#             */
/*   Updated: 2025/03/22 19:22:23 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*new_test_commands(char **paths, char *str)
{
	int		i;
	char	*path_test;
	
	i = 0;
	//if (ft_empty(data) == 1)
	//	return (1);
	//if (only_space(data->input) == 0)
	//	return (1);
	//if (data->path == NULL)
	//	return (1);
	while (paths[i])
	{
		path_test = ft_join(paths[i], str);
		if (access(path_test, F_OK | X_OK) == 0)
		{
			// printf("good path founded : {%s}\n", good_path);
			return (path_test);
		}
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
    return fd;
}


int is_builtin(char *cmd)
{
    if (ft_strlcmp(cmd, "echo") == 0 ||
        ft_strlcmp(cmd, "cd") == 0 ||
        ft_strlcmp(cmd, "exit") == 0 ||
        ft_strlcmp(cmd, "export") == 0 ||
        ft_strlcmp(cmd, "pwd") == 0)
    {
        return (1);
    }
    return (0);
}

int execute_builtin(t_cmd *cmd, t_data *data)
{
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
    // else if (ft_strlcmp(cmd->args[0], "env") == 0)
    // {
    //     show_tab(cmd->envp);
    //     return (1);
    // }
    return (0);
}

// void	execute_cmds(t_cmd *cmds, char **paths)
// {
//     int		fd_in;
//     int		fd_pipe[2];
// 	int		fd;
// 	char	*good_path;
// 	pid_t	pid;
// 	t_file	*current_file;
// 	t_cmd	*current_cmd;

// 	current_cmd = cmds;
// 	fd_in = 0;
//     // rajouter execute_builtin
//     while (current_cmd)
//     {
//         if (current_cmd->next)
//             pipe(fd_pipe);            
//         pid = fork();
//         if (pid == 0)
//         {
//             if (current_cmd->files)
//             {
//                	current_file = current_cmd->files;
//                 while (current_file)
//                 {
//                     fd = open_file(current_file->path, current_file->mode);
//                     if (fd == -1)
//                     {
//                         perror("open");
//                         exit(1);
//                     }
//                     if (current_file->mode == INFILE || current_file->mode == HEREDOC)
//                         dup2(fd, STDIN_FILENO);
//                     else
//                         dup2(fd, STDOUT_FILENO);
//                     close(fd);
//                     current_file = current_file->next;
//                 }
//             }
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
// 			good_path = new_test_commands(paths,  current_cmd->args[0]);
// 			if (good_path != (NULL))
//             {
// 				execve(good_path, current_cmd->args, NULL);
// 				perror("execve");
// 				exit(1);
// 			}
// 			else
// 				printf("minishell: %s: command not found\n", current_cmd->args[0]);
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


//Mini_shell$ pwd
///home/maissat/Desktop/minishellm21/built_in			
//Mini_shell$ pwd | cd ..
//Mini_shell$ pwd
///home/maissat/Desktop/minishellm21

void	execute_cmds(t_data *data, t_cmd *cmds, char **paths)
{
    int     fd_in = 0;
    int     fd_pipe[2];
    int     fd;
    char    *good_path;
    pid_t   pid;
    t_file  *current_file;
    t_cmd   *current_cmd;
	
	current_cmd = cmds;
    while (current_cmd)
    {
        if (current_cmd->next)
            pipe(fd_pipe);
		else if (execute_builtin(current_cmd, data) == 1)
			return ;
			
        pid = fork();
        if (pid == 0)
        {
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
            if (current_cmd->files)
            {
                current_file = current_cmd->files;
                while (current_file)
                {
                    fd = open_file(current_file->path, current_file->mode);
                    if (fd == -1)
                    {
                        perror("open");
                        exit(1); // free and exit
                    }
                    if (current_file->mode == INFILE || current_file->mode == HEREDOC)
                        dup2(fd, STDIN_FILENO);
                    else
                        dup2(fd, STDOUT_FILENO);
                    close(fd);
                    current_file = current_file->next;
                }
            }
          	if (execute_builtin(current_cmd, data) == 1)
                exit(0);
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
        else if (pid > 0)
        {
            waitpid(pid, NULL, 0);
            if (fd_in != 0)
                close(fd_in);
            if (current_cmd->next)
            {
                close(fd_pipe[1]);
                fd_in = fd_pipe[0];
            }
        }
        current_cmd = current_cmd->next;
    }
}
