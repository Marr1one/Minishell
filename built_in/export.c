/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 19:31:51 by maissat           #+#    #+#             */
/*   Updated: 2025/03/09 20:32:13 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_export_compatibility(char *str)
{
	int	i;
	int	quote;
	
	i = 0;
	quote = 0;
	while (is_alpha(str[i]) == 0)
		i++;
	if (str[i] == '=')
		i++;
	else
		return (1);
	if (str[i] == '"')
		quote = 1;
	i++;
	while (is_alpha(str[i]) == 0 || str[i] == '/')
		i++;
	if (str[i] == '\0')
		return (0);
	else if ((quote == 1 && str[i] == '"'))
		return (0);
	else
		return (1);
}

void	export(t_data *data)
{
	if (get_nbr_node(data->list) == 1)
			show_tab_export(data->envp);
	else
	{
		data->list = data->list->next;
		while(data->list != NULL)
		{
			if (check_export_compatibility(data->list->content) == 0)
			{
				printf("Good format!\n");
				if (check_change(data, data->list->content) == 1)
					return ;
				else
					data->envp = add_export(data, data->list->content);
			}
			else
				printf("Bad format!\n");
			data->list = data->list->next;
		}
	}
}