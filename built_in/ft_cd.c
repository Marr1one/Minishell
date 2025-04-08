/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 23:02:08 by maissat           #+#    #+#             */
/*   Updated: 2025/04/08 15:58:01 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_error(char *message)
{
	printf("minishell: %s\n", message);
	return (1);
}

char	*search_oldpwd(t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], "OLDPWD=", 7) == 0)
			return (take_after(data->envp[i], '='));
		i++;
	}
	return (NULL);
}

void	update_oldpwd(t_data *data, char *pwd)
{
	int		i;
	char	*new_oldpwd;

	new_oldpwd = ft_join("OLDPWD=", pwd);
	i = 0;
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], "OLDPWD=", 7) == 0)
		{
			data->envp[i] = new_oldpwd;
			return ;
		}
		i++;
	}
}

void	update_pwd(t_data *data, char *pwd)
{
	int		i;
	char	*new_pwd;

	new_pwd = ft_join("PWD=", pwd);
	i = 0;
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], "PWD=", 4) == 0)
		{
			data->envp[i] = new_pwd;
			return ;
		}
		i++;
	}
}

int	ft_cd(t_cmd *cmd, t_data *data)
{
	char	current_pwd[1024];
	char	pwd_after[1024];
	char	*target_dir;

	if (list_len(data->list) > 2)
		return (printf("minishell: cd: too many arguments\n"), 1);
	if (!getcwd(current_pwd, sizeof(current_pwd)))
		return (printf("minishell: getcwd\n"), 1);
	data->args = cmd->args;
	target_dir = get_target_dir(data);
	if (!target_dir)
		return (1);
	if (chdir(target_dir) != 0)
		return (print_error("cd"), 1);
	update_oldpwd(data, current_pwd);
	if (!getcwd(pwd_after, sizeof(pwd_after)))
		return (print_error("getcwd"), 1);
	update_pwd(data, pwd_after);
	data->exit_status = 0;
	return (0);
}
