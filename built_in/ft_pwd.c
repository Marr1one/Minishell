/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:55:00 by maissat           #+#    #+#             */
/*   Updated: 2025/03/06 16:56:25 by maissats          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	ft_pwd(t_data *data)
// {
// 	char path[1024];


//     if (getcwd(path, sizeof(path)) != NULL)
// 	{
// 		data->exit_status = 0;
//         printf("%s\n", path);
// 	}
//     else
// 	{
// 		data->exit_status = 1;
//         perror("getcwd");
// 	}
// }

void ft_pwd(t_cmd *cmd)
{
    char cwd[1024];

    // Si vous souhaitez vérifier la présence d'arguments et éventuellement afficher une erreur :
    if (cmd->args[1] != NULL)
    {
        printf("ft_pwd: too many arguments\n");
        return;
    }

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("ft_pwd");
    }
}
