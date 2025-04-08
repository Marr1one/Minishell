/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 18:48:42 by maissat           #+#    #+#             */
/*   Updated: 2025/04/08 18:05:01 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	ft_echo(t_cmd *current_cmd)
{
	int		option_n;
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
		if (args[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (option_n == 0)
		printf("\n");
}
