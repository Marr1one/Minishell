/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 13:19:59 by maissat           #+#    #+#             */
/*   Updated: 2025/02/01 13:54:34 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_cd(t_data data)
{
	if (chdir(data.args[1]) != 0 )
	{
		printf("chdir == 1\n");
		perror("cd");
	}
}

void	ft_exit(void)
{
	exit(0);
}

void ft_echo(t_data data)
{
	int	i;
	int	n;
	
	i = 1;
	n = 0;
	if (ft_strncmp(data.args[1], "-n", ft_strlen(data.args[1])) == 0)
	{
		n = 1;
		i++;
	}
	while (data.args[i])
	{
		printf("%s", data.args[i]);
		if (data.args[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (n == 0)
		printf("\n");
}

int	check_builtin(t_data data)
{
	if (ft_strncmp(data.args[0], "exit", ft_strlen(data.args[0])) == 0)
	{
		ft_exit();
		return (1);
	}
	if (ft_strncmp(data.args[0], "echo", ft_strlen(data.args[0])) == 0)
	{
		ft_echo(data);
		return (1);
	}
	if (ft_strncmp(data.args[0], "cd", ft_strlen(data.args[0])) == 0)
	{
		ft_cd(data);
		return (1);
	}
	return (0);
	
}