/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:16:21 by maissat           #+#    #+#             */
/*   Updated: 2025/02/09 02:19:13 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirect(t_data *data, int	i, char **envp)
{
	int fd;
	pid_t pid;

	data->path = ft_split(get_path_env(envp), ':');
	data->path = add_slash_all(data->path);
	if (test_commands(data) == 0)
	{
		pid = fork();
		if (pid == 0)
		{
			fd = open(data->args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("open");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			execve(data->command_path, data->cmd_args, envp);
			perror("execve");
			exit(1);
		}
		waitpid(pid, NULL, 0);
	}
}

char	**create_cmd_args(t_data *data, int i)
{
	int	j;
	char **res;

	res = malloc(i * sizeof(char *));
	j = 0;
	while (j < i)
	{
		res[j] = data->args[j];
		j++;
	}
	res[j] = NULL;
	return (res);
}

void	handle_append(t_data *data, int i)
{
	int fd;
	pid_t pid;

	data->path = ft_split(get_path_env(data->envp), ':');
	data->path = add_slash_all(data->path);
	if (test_commands(data) == 0)
	{
		pid = fork();
		if (pid == 0)
		{
			fd = open(data->args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror("open");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			execve(data->command_path, data->cmd_args, data->envp);
			perror("execve");
			exit(1);
		}
		waitpid(pid, NULL, 0);	
	}
}

void		handle_back(t_data *data, int i)
{
	int fd;
	pid_t pid;

	data->path = ft_split(get_path_env(data->envp), ':');
	data->path = add_slash_all(data->path);
	if (test_commands(data) == 0)
	{
		pid = fork();
		if (pid == 0)
		{
			fd = open(data->args[i + 1], O_RDONLY);
			if (fd == -1)
			{
				perror("open");
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			execve(data->command_path, data->cmd_args, data->envp);
			perror("execve");
			exit(1);
		}
		waitpid(pid, NULL, 0);	
	}
}

int	case_redirection(t_data *data, char **envp)
{
	t_token	*list;
	
	list = data->list;
	while (list != NULL)
	{
		if (ft_strlcmp(list->content, ">") == 0)
		{
			//printf("on trouve >\n");
			if (!list->next || !(*list->next->content))
			{
				printf("Nul part ou rediriger!\n");
				return (0);
			}
			data->cmd_args = create_cmd_args(data, list->index);
			handle_redirect(data, list->index, envp);
			return (0);	
		}
		else if (ft_strlcmp(list->content, "<") == 0)
		{
			//printf("in < case\n");
			//printf("on trouve >\n");
			if (!list->next || !(*list->next->content))
			{
				printf(" Rien apres le <!\n");
				return (0);
			}
			data->cmd_args = create_cmd_args(data, list->index);
			handle_back(data, list->index);
			//printf("ICI!\n");
			return (0);	
		}
		else if (ft_strlcmp(list->content, ">>") == 0)
		{
			//printf("in < case\n");
			//printf("on trouve >\n");
			if (!list->next || !(*list->next->content))
			{
				printf(" Rien apres le >>!\n");
				return (0);
			}
			data->cmd_args = create_cmd_args(data, list->index);
			handle_append(data, list->index);
			//printf("ICI!\n");
			return (0);	
		}
		list = list->next;
	}
	return (1);
}

int	only_space(char *input)
{
	int	i;

	i = 0;	
	while (input[i])
	{
		if (input[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

int	test_commands(t_data *data)
{
	int	i;
	char *path_test;
	
	i = 0;
	if (only_space(data->input) == 0)
		return (1);
	while (data->path[i])
	{
		path_test = ft_join(data->path[i], data->args[0]);
		if (access(path_test, F_OK | X_OK) == 0)
		{
			data->command_path = path_test;
			return (0);
		}
		i++;
	}
	return (1);
}

int		check_empty(t_data data)
{
	if (!data.list)
		return (1);
	if (data.list->content[0] == '\0')
	{
		return (1);
	}
	return (0);
}

void	parsing(char *input, char **envp, t_data *data)
{
	pid_t	pid;
	int		status;
	
	// show_tab(data->args);
	remove_quotes_all(data);
	if (check_empty(*data) == 1)
	{
		//printf("dans ce cas la\n");
		//printf("minishell: %s: command not found\n", data->args[0]);
		return ;
	}
	//printf("after remove\n");
	//show_tab(data->args);
	if (check_builtin(data) != 0)
		return;
	if (input[0] == '/' || (input[0] == '.' && input[1] == '/'))
	{
		if (access(input, F_OK) != 0)
		{
            printf("minishell: %s: No such file or directory\n", input);
            return;
        }
        if (access(input, X_OK) != 0)
        {
            printf("minishell: %s: Permission denied\n", input);
            return;
        }
		pid = fork();
		if (pid == 0)
		{
			execve(input, data->args, envp);
			perror("execve");
			exit(1);
		}
		waitpid(pid, &status, 0);
		return ;
	}
	data->path = ft_split(get_path_env(envp), ':');
	data->path = add_slash_all(data->path);
	if (test_commands(data) == 0)
	{
		//printf("dans ce cas la \n");
		//show_tab(data->args);
		// printf("on a trouver un chemin qui marche !\n");
		pid = fork();
		if (pid == 0)
		{
			execve(data->command_path, data->args, envp);
			perror("execve");
			exit(1);
		}
		waitpid(pid, &status, 0);
		return;
	}
	if (only_space(data->input) == 0)
		return ;
	//printf("on est ici donc test command == 1\n");
	printf("minishell: %s: command not found\n", input);
}
