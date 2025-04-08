/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 03:40:20 by maissat           #+#    #+#             */
/*   Updated: 2025/04/08 15:10:30 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_joinunset(char *str)
{
	int		i;
	int		j;
	int		len;
	char	*joineg;

	len = ft_strlen(str);
	i = 0;
	j = 0;
	joineg = ft_malloc(sizeof(char) * (len + 2));
	if (!joineg)
		return (NULL);
	while (str[i])
	{
		joineg[j] = str[i];
		j++;
		i++;
	}
	joineg[j++] = '=';
	joineg[j] = '\0';
	return (joineg);
}

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

void	check_unset(t_data *data, char	*str)
{
	int		i;
	char	*join_eg;

	i = 0;
	join_eg = ft_joinunset(str);
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], join_eg, ft_strlen(join_eg)) == 0)
		{
			data->envp = ft_unset(data, i);
			break ;
		}
		i++;
	}
	data->exit_status = 0;
}

char	**ft_unset(t_data *data, int save)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	while (data->envp[i])
		i++;
	new_env = ft_malloc(sizeof(char *) * i);
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (data->envp[i])
	{
		if (i != save)
		{
			new_env[j] = ft_strdup(data->envp[i]);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}
