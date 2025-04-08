/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:10:49 by braugust          #+#    #+#             */
/*   Updated: 2025/04/08 04:28:52 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_command_path(t_data *data, char **paths, t_cmd *current_cmd)
{
	char	*good_path;

	if (test_relative_path(current_cmd->args[0]) == 0)
	{
		execve(current_cmd->args[0], current_cmd->args, data->envp);
		print_error("execve");
		free_all(data->gc);
		exit(1);
	}
	good_path = new_test_commands(paths, current_cmd->args[0]);
	if (good_path != NULL)
	{
		execve(good_path, current_cmd->args, data->envp);
		print_error("execve");
		free_all(data->gc);
		exit(1);
	}
	else
	{
		printf("minishell: %s: command not found\n", current_cmd->args[0]);
		free_all(data->gc);
		exit(127);
	}
}

void	execute_command(t_data *data, t_cmd *cmd)
{
	char	**paths;

	if (!cmd->args || !cmd->args[0])
	{
		free_all(data->gc);
		exit(0);
	}
	if (execute_builtin_child(cmd, data))
	{
		free_all(data->gc);
		exit(0);
	}
	paths = ft_split(get_path_env(data->envp), ':');
	paths = add_slash_all(paths);
	execute_command_path(data, paths, cmd);
}
