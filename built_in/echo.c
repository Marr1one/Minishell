/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 18:48:42 by maissat           #+#    #+#             */
/*   Updated: 2025/03/23 00:08:13 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void ft_echo(t_data data)
// {
// 	int		n;
// 	t_token	*list;
// 	int		i;
	
// 	list = data.list->next;
// 	n = 0;
// 	i = 0;
// 	while (list && list->content[0] == '-' && list->content[1] == 'n')
// 	{
// 		i = 2;
// 		while (list->content[i] == 'n')
// 			i++;
// 		if (list->content[i] != '\0')
// 			break;
// 		n = 1;
// 		list = list->next;
// 	}
// 	while (list != NULL)
// 	{
// 		if (list->next == NULL)
// 			printf("%s", list->content);
// 		else
// 			printf("%s ", list->content);
// 		if (data.args[i + 1] != NULL)
// 			printf(" ");
// 		i++;
// 		list = list->next;
// 	}
// 	if (n == 0)
// 		printf("\n");
// }


void ft_echo(t_cmd *current_cmd)
{
    int     option_n;
    char	**args;  // on suppose que data.list pointe sur "echo"
	int		i;

	args = current_cmd->args;
	option_n = 0;
    // Traitement des options -n
    // while (args && args[1][0] == '-' && args[1][1] == 'n')
    // {
    //     int j = 1;
    //     while (token->content[j])
    //     {
    //         if (token->content[j] != 'n')
    //             break;
    //         j++;
    //     }
    //     if (token->content[j] != '\0')
    //         break;
    //     option_n = 1;
    //     token = token->next;
    // }
	if (args[1] && args[1][0] == '-' && args[1][1] == 'n')		//truc a ajouter, faire que echo -n -n salut marche bien
	{
		i = 2;
		while (args && args[1][i] == 'n')
			i++;
		if (args[1][i] == '\0' || args[1][i] == ' ')
			option_n = 1;
		else
			option_n = 0;
	}
	if (option_n == 0)
		i= 1;
	else
		i = 2;
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i  + 1] != NULL)
            printf(" ");
        i++;
    }
    if (option_n == 0)
        printf("\n");
}
