/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 18:48:42 by maissat           #+#    #+#             */
/*   Updated: 2025/03/31 20:03:19 by braugust         ###   ########.fr       */
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


int	is_option_n(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' && str[1] == 'n')
	{
		i = 2;
		while (str[i] && str[i] == 'n')
			i++;
		if (str[i] != '\0')
			return (1);
		return (0);
	}
	return (1);
}

void ft_echo(t_cmd *current_cmd)
{
    int     option_n;
    char	**args;
	int		i;

	args = current_cmd->args;
	option_n = 0;
	i = 1;
	while (args[i] && is_option_n(args[i]) == 0)
	{
		i++;
		option_n = 1;
	}
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