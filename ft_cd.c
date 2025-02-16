/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 23:02:08 by maissat           #+#    #+#             */
/*   Updated: 2025/02/16 23:25:19 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**update_oldpwd(t_data *data, char *pwd)
{
	int	i;
	char	**new_env;
	

	i = 0;
	while (data->envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while(data->envp[i])
	{
		if (ft_strncmp(data->envp[i], "OLDPWD=", 7) == 0)
		{
			new_env[i] = ft_join("OLDPWD=", pwd);
		}
		new_env[i] = ft_strdup(data->envp[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void ft_cd(t_data *data)
{
	char	*old_pwd;
	char	current_pwd[1024];

	if (getcwd(current_pwd, sizeof(current_pwd)) == NULL)
	{
		perror("getcwd");
	}
	// printf("current pwd = %s\n", current_pwd);
	old_pwd = getenv("OLDPWD");
	// printf("old_pwd = %s\n", old_pwd);
	if (data->args[1] && ft_strlcmp(data->args[1], "-") == 0)
	{
		printf("dans ce cas la\n");
		if (!old_pwd)
		{
			printf("minishell: cd: OLDPWD not set\n");
			data->exit_status = 1;
			return ;
		}
		if (chdir(old_pwd) != 0)
		{
			perror("cd");
			data->exit_status = 1;	
		}
		data->exit_status = 0;
	}
	else if (data->args[1] && chdir(data->args[1]) != 0)
	{
		data->exit_status = 1;
		perror("cd");
	}
	else
		data->exit_status = 0;
	data->envp = update_oldpwd(data, current_pwd);
}