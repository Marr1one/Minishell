/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:09:08 by braugust          #+#    #+#             */
/*   Updated: 2025/04/08 15:20:34 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	count_cmds(t_cmd *cmds)
{
	int		i;
	t_cmd	*curr_cmd;

	i = 0;
	curr_cmd = cmds;
	while (curr_cmd)
	{
		i++;
		curr_cmd = curr_cmd->next;
	}
	return (i);
}

int	setup_cmd_pipe(int *fd_pipe, t_cmd *cmd)
{
	if (cmd->next)
	{
		if (pipe(fd_pipe) == -1)
		{
			perror("pipe");
			return (0);
		}
	}
	else
	{
		fd_pipe[0] = -1;
		fd_pipe[1] = -1;
	}
	return (1);
}
