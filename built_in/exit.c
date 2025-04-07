/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 16:46:56 by maissat           #+#    #+#             */
/*   Updated: 2025/04/03 15:13:24 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_overflow(char *arg)
{
	int		i;
	int		len;
	char	*max_val = "9223372036854775807";

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i] && arg[i] == '0')
		i++;
	len = ft_strlen(arg + i);
	if (len > 19)
		return (1);
	else if (len == 19)
	{
		if (ft_strncmp(arg + i, max_val, 19) > 0)
			return (1);
	}
	return (0);
}

int	validate_exit_arg(char *arg, int *status)
{
	if (is_numeric(arg) == 0 || is_overflow(arg) == 1)
	{
		printf("minishell: exit: %s: numeric argument required\n", arg);
		*status = 2;
		return (0);
	}
	*status = ft_atoi(arg);
	if (*status < 0 || *status > 255)
		*status = *status % 256;
	return (1);
}

int	check_exit_args_count(char **args, t_data *data)
{
	if (count_args(args) > 2)
	{
		printf("minishell: exit: too many arguments\n");
		data->exit_status = 1;
		return (1);
	}
	return (0);
}

int	process_exit_args(t_cmd *cmd, t_data *data, int *status)
{
	int	valid;

	*status = data->exit_status;
	if (!cmd->args[1])
		return (0);
	valid = validate_exit_arg(cmd->args[1], status);
	if (valid && check_exit_args_count(cmd->args, data))
		return (1);
	return (0);
}

void	ft_exit(t_cmd *cmd, t_data *data)
{
	int	status;
	int	should_return;

	printf("exit\n");
	should_return = process_exit_args(cmd, data, &status);
	if (should_return)
		return ;
	free_all(data->gc);
	exit(status);
}
