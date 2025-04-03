/* ************************************************************************** */
/*					                                                  */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 18:33:35 by braugust          #+#    #+#             */
/*   Updated: 2025/04/03 18:33:47 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_cmd *cmd)
{
	char	cwd[1024];

	if (cmd->args[1] != NULL)
		return (printf("ft_pwd: too many arguments\n"), 1);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return (printf("%s\n", cwd), 0);
	else
		return (perror("ft_pwd"), 1);
}
