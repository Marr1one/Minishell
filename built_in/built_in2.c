/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:08:35 by braugust          #+#    #+#             */
/*   Updated: 2025/04/08 03:46:20 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

char	*get_target_dir(t_data *data)
{
	char	*home;
	char	*oldpwd;

	if (!data->args[1])
	{
		home = check_env(data, "HOME");
		if (!home)
			return (print_error("cd: HOME not set"), NULL);
		return (home);
	}
	else if (ft_strlcmp(data->args[1], "-") == 0)
	{
		oldpwd = search_oldpwd(data);
		if (!oldpwd)
		{
			print_error("cd: OLDPWD not set");
			data->exit_status = 1;
			return (NULL);
		}
		printf("%s\n", oldpwd);
		return (oldpwd);
	}
	return (data->args[1]);
}
