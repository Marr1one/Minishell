/* ************************************************************************** */
/*												                            */
/*                                                        :::      ::::::::   */
/*   builtin_advanced.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:00:29 by braugust          #+#    #+#             */
/*   Updated: 2025/04/03 21:13:00 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_advanced_builtin(t_cmd *cmd, t_data *data)
{
	if (ft_strlcmp(cmd->args[0], "export") == 0)
	{
		data->exit_status = ft_export(cmd, data);
		return (1);
	}
	if (ft_strlcmp(cmd->args[0], "cd") == 0)
	{
		data->exit_status = ft_cd(cmd, data);
		return (1);
	}
	if (ft_strlcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(cmd, data), 1);
	return (0);
}

int	execute_builtin_child(t_cmd *cmd, t_data *data)
{
	if (ft_strlcmp(cmd->args[0], "unset") == 0)
		return (execute_unset(cmd, data));
	if (execute_simple_builtin(cmd, data))
		return (1);
	if (execute_advanced_builtin(cmd, data))
		return (1);
	return (0);
}

void	handle_single_builtin(t_data *data, t_cmd *current_cmd)
{
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (handle_file_redirections(current_cmd) == 0)
	{
		if (ft_strlcmp(current_cmd->args[0], "exit") == 0)
		{
			dup2(stdin_backup, STDIN_FILENO);
			dup2(stdout_backup, STDOUT_FILENO);
			close(stdin_backup);
			close(stdout_backup);
		}
		execute_builtin_child(current_cmd, data);
	}
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

int	check_single_builtin(t_data *data, t_cmd *cmds)
{
	if (count_cmds(cmds) == 1 && ncheck_builtin(cmds))
	{
		handle_single_builtin(data, cmds);
		return (1);
	}
	return (0);
}
