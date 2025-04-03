/* ************************************************************************** */
/*				                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_simple.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:58:55 by braugust          #+#    #+#             */
/*   Updated: 2025/04/03 21:14:28 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

int	execute_unset(t_cmd *cmd, t_data *data)
{
	int	i;
	int	exit_status;

	if (!cmd->args[1])
	{
		data->exit_status = 0;
		return (1);
	}
	exit_status = 0;
	i = 1;
	while (cmd->args[i])
	{
		check_unset(data, cmd->args[i]);
		if (data->exit_status == 1)
			exit_status = 1;
		i++;
	}
	data->exit_status = exit_status;
	return (1);
}

int	execute_simple_builtin(t_cmd *cmd, t_data *data)
{
	if (ft_strlcmp(cmd->args[0], "echo") == 0)
	{
		data->exit_status = 0;
		return (ft_echo(cmd), 1);
	}
	if (ft_strlcmp(cmd->args[0], "pwd") == 0)
	{
		data->exit_status = ft_pwd(cmd);
		return (1);
	}
	if (ft_strlcmp(cmd->args[0], "env") == 0)
	{
		data->exit_status = show_env(data->envp);
		return (1);
	}
	return (0);
}

int	ncheck_builtin(t_cmd *cmd)
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
