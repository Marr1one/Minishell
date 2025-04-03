/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 16:46:56 by maissat           #+#    #+#             */
/*   Updated: 2025/04/03 14:58:10 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_exit(t_cmd *cmd, t_data *data)
{
	int	status;

	status = data->exit_status;
	printf("exit\n");
	if (cmd->args[1])
	{
		if (is_numeric(cmd->args[1]) == 0)
		{
			printf("minishell: exit: %s: numeric argument required\n", cmd->args[1]);
			status = 2;
		}
		else 
		{
			status = ft_atoi(cmd->args[1]);	
			if (status < 0 || status > 255)
				status = status % 256;
			if (count_args(cmd->args) > 2)
			{
				printf("minishell: exit: too many arguments\n");
				data->exit_status = 1;
				return ;	
			}
		}
	}
	free_all(data->gc);
	exit(status);
}
