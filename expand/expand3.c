/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:12:20 by braugust          #+#    #+#             */
/*   Updated: 2025/04/09 16:09:44 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	expand_argument(char **arg, t_data *data)
{
	if (!arg || !(*arg))
		return ;
	*arg = expand_string(*arg, data);
}

void	expand_cmd_args(t_cmd *cmd, t_data *data)
{
	int	i;

	i = 0;
	if (!cmd || !cmd->args)
		return ;
	while (cmd->args[i])
	{
		expand_argument(&cmd->args[i], data);
		i++;
	}
}

void	expand_file_path(t_file *file, t_data *data)
{
	if (!file)
		return ;
	if (file->mode == HEREDOC)
	{
		file->path = quoteless_string(file->path);
		return ;
	}
	file->path = expand_string(file->path, data);
}

void	expand_cmd_files(t_cmd *cmd, t_data *data)
{
	t_file	*cur;

	cur = cmd->files;
	while (cur)
	{
		expand_file_path(cur, data);
		cur = cur->next;
	}
}

void	expand_all(t_cmd *cmd, t_data *data)
{
	t_cmd	*cur;

	cur = cmd;
	while (cur)
	{
		expand_cmd_args(cur, data);
		expand_cmd_files(cur, data);
		cur = cur->next;
	}
}
