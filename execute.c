/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:15:55 by maissat           #+#    #+#             */
/*   Updated: 2025/03/18 14:49:39 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*new_test_commands(char **paths, char *str)
{
	int		i;
	char	*path_test;
	char	*good_path;
	
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
			good_path = ft_strdup(path_test);
			// printf("good path founded : {%s}\n", good_path);
			return (good_path);
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

void execute_builtin(t_data *data, char **args)
{
    if (ft_strlcmp(args[0], "echo") == 0)
    {
        ft_echo(*data);
    }
    else if (ft_strlcmp(args[0], "cd") == 0)
    {
        ft_cd(data);
    }
    else if (ft_strlcmp(args[0], "exit") == 0)
    {
        ft_exit(data);
    }
    else if (ft_strlcmp(args[0], "pwd") == 0)
    {
        ft_pwd(data);
    }
    else if (ft_strlcmp(args[0], "env") == 0)
    {
        show_tab(data->envp);
    }
}


void	execute_cmds(t_cmd *cmds, char **paths)
{
    int		fd_in;
    int		fd_pipe[2];
	int		fd;
	char	*good_path;
	pid_t	pid;
	t_file	*current_file;
	t_cmd	*current_cmd;

	current_cmd = cmds;
	fd_in = 0;
    // rajouter execute_builtin
    while (current_cmd)
    {
        if (current_cmd->next)
            pipe(fd_pipe);            
        pid = fork();
        if (pid == 0)
        {
            if (current_cmd->files)
            {
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
			good_path = new_test_commands(paths,  current_cmd->args[0]);
			if (good_path != (NULL))
            {
				execve(good_path, current_cmd->args, NULL);
				perror("execve");
				exit(1);
			}
			else
				printf("minishell: %s: command not found\n", current_cmd->args[0]);
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

