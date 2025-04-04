/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:14:44 by braugust          #+#    #+#             */
/*   Updated: 2025/04/04 15:51:15 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
** Traite l'expansion d'un argument spécifique
*/
void	expand_argument(char **args, int i, t_data *data)
{
	char	*expanded;

	if (ft_strlcmp(args[i], "$?") == 0)
	{
		expanded = expand_exit_status(data);
		args[i] = expanded;
	}
	else
	{
		expanded = expand_string(args[i], data);
		if (expanded == NULL)
			args = create_new_args(args, i);
		else
			args[i] = expanded;
	}
}

/*
** Traite l'expansion de tous les arguments d'une commande
*/
void	expand_cmd_args(t_cmd *cmd, t_data *data)
{
	int	i;

	i = -1;
	while (cmd->args && cmd->args[++i])
	{
		expand_argument(cmd->args, i, data);
	}
}

/*
** Traite l'expansion du chemin d'un fichier
*/
void	expand_file_path(t_file *file, t_data *data)
{
	char	*expanded;

	if (file->mode != HEREDOC)
	{
		expanded = expand_string(file->path, data);
		file->path = expanded;
	}
	else
	{
		if (ft_strlcmp(file->path, "$?") == 0)
			expanded = expand_exit_status(data);
		else
			expanded = expand_string(file->path, data);
		file->path = expanded;
	}
}

/*
** Traite l'expansion de tous les fichiers d'une commande
*/
void	expand_cmd_files(t_cmd *cmd, t_data *data)
{
	t_file	*current_file;

	current_file = cmd->files;
	while (current_file)
	{
		expand_file_path(current_file, data);
		current_file = current_file->next;
	}
}

/*
** Fonction principale pour l'expansion de toutes les variables
*/
void	expand_all(t_cmd *cmd, t_data *data)
{
	t_cmd	*current_cmd;

	current_cmd = cmd;
	while (current_cmd)
	{
		expand_cmd_args(current_cmd, data);
		expand_cmd_files(current_cmd, data);
		current_cmd = current_cmd->next;
	}
}
