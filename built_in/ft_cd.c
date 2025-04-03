/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 23:02:08 by maissat           #+#    #+#             */
/*   Updated: 2025/04/03 23:56:12 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	print_error(char *message)
{
	printf("minishell: %s\n", message);
	return (1);
}

static char	*get_target_dir(char **args)
{
	char	*home;

	if (!args[1])
	{
		home = getenv("HOME");
		if (!home)
		{
			print_error("cd: HOME not set");
			return (NULL);
		}
		return (home);
	}
	return (args[1]);
}

static int	update_pwd_and_cd(char *target_dir)
{
	char	current_pwd[1024];
	int		chdir_status;

	if (getcwd(current_pwd, sizeof(current_pwd)) == NULL)
		return (print_error("getcwd"));
	chdir_status = chdir(target_dir);
	if (chdir_status != 0)
		return (print_error("cd"));
	return (0);
}

int	ft_cd(t_cmd *cmd)
{
	char	*target_dir;
	int		arg_count;

	arg_count = count_args(cmd->args);
	if (arg_count > 2)
		return (print_error("cd: too many arguments"));
	target_dir = get_target_dir(cmd->args);
	if (!target_dir)
		return (1);
	return (update_pwd_and_cd(target_dir));
}
