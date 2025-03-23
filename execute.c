/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:15:55 by maissat           #+#    #+#             */
/*   Updated: 2025/03/23 16:04:41 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*new_test_commands(char **paths, char *str)
{
	int		i;
	char	*path_test;
	
	if (!paths)
		return (NULL);
	i = 0;
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
        ft_strlcmp(cmd, "pwd") == 0 ||
        ft_strlcmp(cmd, "unset") == 0)
    {
        return (1);
    }
    return (0);
}

int execute_builtin(t_cmd *cmd, t_data *data)
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
    else if (ft_strlcmp(cmd->args[0], "pwd") == 0)
    {
        ft_pwd(cmd);
        return (1);
    }
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

void	execute_cmds(t_data *data, t_cmd *cmds)
{
    int     fd_in = 0;
    int     fd_pipe[2];
    int     fd;
	char	**paths;
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
			paths = ft_split(get_path_env(data->envp), ':');
			paths = add_slash_all(paths);
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
