/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 23:02:08 by maissat           #+#    #+#             */
/*   Updated: 2025/03/25 14:53:28 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	update_oldpwd(t_data *data, char *pwd)
// {
// 	int	i;
// 	char	*new_oldpwd;

// 	new_oldpwd = ft_join("OLDPWD=", pwd);
	

// 	// i = 0;
// 	// while (data->envp[i])
// 	// 	i++;
// 	// new_env = malloc(sizeof(char *) * (i + 1));
// 	i = 0;
// 	while(data->envp[i])								OPTIONNEL LE cd - DEMANDER A MOUSSA OU IDRISSA
// 	{
// 		if (ft_strncmp(data->envp[i], "OLDPWD=", 7) == 0)
// 		{
// 			// free(data->envp[i]);
// 			data->envp[i] = new_oldpwd;
// 		}
// 		// new_env[i] = ft_strdup(data->envp[i]);
// 		i++;
// 	}
// 	// new_env[i] = NULL;
// 	// return (new_env);
// }

// void	update_pwd(t_data *data, char *pwd)
// {
// 	int	i;
// 	char	*new_pwd;
	
// 	new_pwd = ft_join("PWD=", pwd);
// 	// i = 0;
// 	// while (data->envp[i])
// 	// 	i++;
// 	// new_env = malloc(sizeof(char *) * (i + 1));
// 	i = 0;
// 	while(data->envp[i])
// 	{
// 		if (ft_strncmp(data->envp[i], "PWD=", 7) == 0)		OPTIONNEL LE cd - DEMANDER A MOUSSA OU IDRISSA
// 		{
// 			// free(data->envp[i]);
// 			data->envp[i] = new_pwd;
// 		}
// 		// new_env[i] = ft_strdup(data->envp[i]);
// 		i++;
// 	}
// 	// new_env[i] = NULL;
// 	// return (new_env);
// }

// char	*search_oldpwd(t_data *data)
// {
// 	int		i;
// 	char	*res;

// 	i = 0;
// 	while (data->envp[i])
// 	{
// 		if (ft_strncmp(data->envp[i], "OLDPWD=", 7) == 0)		OPTIONNEL LE cd - DEMANDER A MOUSSA OU IDRISSA
// 		{
// 			res = take_after(data->envp[i], '=');
// 			return (res);
// 		}
// 		i++;
// 	}
// 	return (NULL);
// }

// void ft_cd(t_cmd *cmd)
// {
// 	// char	*old_pwd;
// 	char	current_pwd[1024];
// 	// char	pwd_after[1024];

// 	if(count_args(cmd->args) > 2)
// 	{
// 		printf("minishell: cd: too many arguments\n");
// 		return ;
// 	}
// 	if (getcwd(current_pwd, sizeof(current_pwd)) == NULL)
// 		return (perror("getcwd"));
// 	// printf("current pwd = %s\n", current_pwd);
// 	// old_pwd = search_oldpwd(data);		OPTIONNEL LE cd - DEMANDER A MOUSSA OU IDRISSA
// 	//printf("old_pwd = %s\n", old_pwd);
// 	// if (data->args[1] && ft_strlcmp(data->args[1], "-") == 0)
// 	// {
// 	// 	if (!old_pwd)
// 	// 	{
// 	// 		//printf("minishell: cd: OLDPWD not set\n");
// 	// 		data->exit_status = 1;
// 	// 		return ;
// 	// 	}
// 	// 	if (chdir(old_pwd) != 0)
// 	// 	{
// 	// 		perror("cd");
// 	// 		data->exit_status = 1;	
// 	// 	}
// 	// 	printf("%s\n", old_pwd);
// 	// 	data->exit_status = 0;
// 	// }
// 	else if (cmd->args[1] && chdir(cmd->args[1]) != 0)
// 	{
// 		// data->exit_status = 1;			GERER LES EXITS STATUS APRES
// 		perror("cd");
// 	}
// 	// else
// 		// data->exit_status = 0;
// 	// update_oldpwd(data, current_pwd);		OPTIONNEL LE cd - DEMANDER A MOUSSA OU IDRISSA
// 	// if (getcwd(pwd_after, sizeof(pwd_after)) == NULL)
// 	// {
// 	// 	perror("getcwd");
// 	// }
// 	// update_pwd(data, pwd_after);
// }


void	ft_cd(t_cmd *cmd)
{
	char	*target_dir;
	char	current_pwd[1024];

	if (count_args(cmd->args) > 2)
	{
		printf("minishell: cd: too many arguments\n");
		return ;
	}
	if (!cmd->args[1])
	{
		target_dir = getenv("HOME");
		if (!target_dir)
		{
			printf("minishell: cd: HOME not set\n");
			return ;
		}
	}
	else
		target_dir = cmd->args[1];

	if (getcwd(current_pwd, sizeof(current_pwd)) == NULL)
	{
		perror("minishell: getcwd");
		return ;
	}

	if (chdir(target_dir) != 0)
		perror("minishell: cd");
}
