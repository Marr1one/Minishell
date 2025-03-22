/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 19:31:51 by maissat           #+#    #+#             */
/*   Updated: 2025/03/22 19:36:26 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_export_compatibility(char *str)
{
	int	i;
	int	quote;
	
	i = 0;
	quote = 0;
	while (is_alpha(str[i]))
		i++;
	if (str[i] == '=')
		i++;
	else
		return (1);
	if (str[i] == '"')
		quote = 1;
	i++;
	while (is_alpha(str[i]) || str[i] == '/')
		i++;
	if (str[i] == '\0')
		return (0);
	else if ((quote == 1 && str[i] == '"'))
		return (0);
	else
		return (1);
}

void	ft_export(t_cmd *cmd, t_data *data)
{
	int		i;
	char	**args;

	args = cmd->args;
	if (count_args(args) == 1)
		return (show_tab_export(data->envp));
	i = 1;
	while (args[i])
	{
		if (check_export_compatibility(args[i]) == 0)
		{
			if (check_change(data, cmd->args[i]) == 1)
				return ;
			else
				data->envp = add_export(data, cmd->args[i]);
		}
		else
			printf("Bad format!\n");
		i++;
	}
}