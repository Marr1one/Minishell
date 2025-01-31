/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:16:21 by maissat           #+#    #+#             */
/*   Updated: 2025/01/29 17:38:18 by maissat          ###   ########.fr       */
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
		path_test = ft_join(data->path[i], data->input);
		if (access(path_test, F_OK | X_OK) == 0)
		{
			data->command = path_test;
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
			execve(input, ft_split(input, ' '), envp);
			perror("execve");
			exit(1);
		}
		waitpid(pid, &status, 0);
		return;
	}
	data->path = ft_split(get_path_env(envp), ':');
	data->path = add_slash_all(data->path);
	if (test_commands(data) != 1)
	{
		pid = fork();
		if (pid == 0)
		{
			execve(data->command, ft_split(data->command, ' '), envp);
			perror("execve");
			exit(1);
		}
		waitpid(pid, &status, 0);
		return;
	}
}