/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 18:48:42 by maissat           #+#    #+#             */
/*   Updated: 2025/02/15 19:14:27 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_echo(t_data data)
{
	int		n;
	t_token	*list;
	
	list = data.list->next;
	n = 0;
	while (ft_strcmp(list->content, "-n") == 0)
	{
		n = 1;
		list = list->next;
	} 	
	while (list != NULL)
	{
		if (list->next == NULL)
			printf("%s", list->content);
		else
			printf("%s ", list->content);
		// if (data.args[i + 1] != NULL)
		// 	printf(" ");
		// i++;
		list = list->next;
	}
	if (n == 0)
		printf("\n");
}