/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 19:38:38 by maissat           #+#    #+#             */
/*   Updated: 2025/03/06 16:48:16 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	test_commands(t_data *data)
{
	int		i;
	char	*path_test;
	
	i = 0;
	if (ft_empty(data) == 1)
		return (1);
	if (only_space(data->input) == 0)
		return (1);
	if (data->path == NULL)
		return (1);
	while (data->path[i])
	{
		path_test = ft_join(data->path[i], data->list->content);
		if (access(path_test, F_OK | X_OK) == 0)
		{
			data->command_path = ft_strdup(path_test);
			// free(path_test);
			return (0);
		}
		i++;
	}
	// free(path_test);
	return (1);
}
void	exec_command(t_data *data)
{
	pid_t	pid;
	int		status;
	data->args = list_to_args(data);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		execve(data->command_path, data->args, data->envp);
		perror("execve");
		exit(127);
	}	
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status)) // cas ou on a fait ctrl +c pendant la commande (par ex un sleep), la ca a pas vraiment
	// dinteret cest juste pour faire comme bash
	{
		int signal_number = WTERMSIG(status);
		printf("Process terminated by signal: %d\n", signal_number);
		data->exit_status = 128 + signal_number;
	}
	else if (WIFEXITED(status)) //cas ou tout se passe bien
	{
		data->exit_status = WEXITSTATUS(status);
		//check_exit_status(data);
	}
	else //securite au cas ou yai un truc innatendu, afin que exit status ai pas une valeur random
	{
		data->exit_status = 128;
		//check_exit_status(data);
	}
	return;
}