/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 18:48:42 by maissat           #+#    #+#             */
/*   Updated: 2025/02/18 19:35:18 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_echo(t_data data)
{
	int		n;
	t_token	*list;
	int		i;
	
	list = data.list->next;
	n = 0;
	while (list && list->content[0] == '-' && list->content[1] == 'n')
	{
		i = 2;
		while (list->content[i] == 'n')
			i++;
		if (list->content[i] != '\0')
			break;
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