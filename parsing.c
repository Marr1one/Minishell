/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:16:21 by maissat           #+#    #+#             */
/*   Updated: 2025/02/01 13:56:46 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	test_commands(t_data *data)
{
	int	i;
	char *path_test;
	
	i = 0;
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

void	parsing(char *input, char **envp, t_data *data)
{
	pid_t	pid;
	int		status;
	if (access(input, F_OK | X_OK) == 0)
	{
		pid = fork();
		if (pid == 0)
		{
			execve(input, data->args, envp);
			perror("execve");
			exit(1);
		}
		waitpid(pid, &status, 0);
		return;
	}
	data->path = ft_split(get_path_env(envp), ':');
	data->path = add_slash_all(data->path);
	if (test_commands(data) == 0)
	{
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
	printf("%s: command not found\n", input);
}