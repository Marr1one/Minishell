/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:09:08 by braugust          #+#    #+#             */
/*   Updated: 2025/04/06 12:49:26 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*new_test_commands(char **paths, char *str)
{
	int		i;
	char	*path_test;

	if (!paths)
		return (NULL);
	if (!*str)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path_test = ft_join(paths[i], str);
		if (access(path_test, F_OK | X_OK) == 0)
			return (path_test);
		i++;
	}
	return (NULL);
}

int	count_cmds(t_cmd *list_cmds)
{
	int		count;
	t_cmd	*current_cmd;

	current_cmd = list_cmds;
	count = 0;
	while (current_cmd)
	{
		count++;
		current_cmd = current_cmd->next;
	}
	return (count);
}

int	test_relative_path(char *path_test)
{
	if (access(path_test, F_OK | X_OK) == 0)
		return (0);
	return (1);
}

void	setup_pipe(t_cmd *cmd, int *fd_pipe)
{
	if (cmd->next)
		pipe(fd_pipe);
}
