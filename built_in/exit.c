/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 16:46:56 by maissat           #+#    #+#             */
/*   Updated: 2025/03/19 02:49:41 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_exit_status(t_data *data)
{
	t_token	*list;

	list = data->list;
	while (list)
	{
		if (ft_strlcmp(list->content, "$?") == 0)
		{
			//printf("$? trouvE!\n");
			//printf("before : list content = {%s}\n", list->content);
			//printf("data.exit_status = {%d}\n", data->exit_status);
			list->content = ft_strdup(ft_itoa(data->exit_status));
			//printf("after ; list.content = {%s}\n", list->content);
		}
		list = list->next;
	}
}

int	return_exit_status(t_data *data)
{
	t_token	*list;

	list = data->list;
	while (list)
	{
		if (ft_strlcmp(list->content, "$?") == 0)
		{
			//printf("$? trouvE!\n");
			//printf("before : list content = {%s}\n", list->content);
			//printf("data.exit_status = {%d}\n", data->exit_status);
			list->content = ft_strdup(ft_itoa(data->exit_status));
			//printf("after ; list.content = {%s}\n", list->content);
			return (1);
		}
		list = list->next;
	}
	return (0);
}

void	ft_exit(t_cmd *cmd)
{
	int	status;

	status = 0;
	printf("exit\n");
	if (cmd->args[1])
	{
		status = ft_atoi(cmd->args[1]);
		if (status < 0 || status > 255)
			status = status % 256;
		if (is_numeric(cmd->args[1]) == 0)
			printf("minishell: exit: %s: numeric argument required\n", cmd->args[1]);
		else if (count_args(cmd->args) > 2)
		{
			printf("minishell: exit: too many arguments\n");
			return ;	
		}
	}
	// show_malloc_list(data->gc);  A FAIRE PLUS TARD
	// free_all(data->gc);
	exit(status);
}