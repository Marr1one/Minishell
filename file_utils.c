/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:10:23 by maissat           #+#    #+#             */
/*   Updated: 2025/04/04 00:05:57 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_file	*add_file(t_file *list_file, t_token *file_tkn, t_type mode)
{
	t_file	*new_file;
	t_file	*last_file;

	new_file = ft_malloc(sizeof(t_file));
	if (!new_file)
		return (NULL);
	new_file->mode = mode;
	new_file->path = file_tkn->content;
	new_file->next = NULL;
	if (list_file == NULL)
		return (new_file);
	else
	{
		last_file = list_file;
		while (last_file->next)
			last_file = last_file->next;
		last_file->next = new_file;
	}
	return (list_file);
}

t_type	save_mode(t_token current_tkn)
{
	t_type	save;

	save = UNKNOWN;
	if (current_tkn.type == HEREDOC)
		save = HEREDOC;
	if (current_tkn.type == INFILE)
		save = INFILE;
	if (current_tkn.type == OUTFILE_APPEND)
		save = OUTFILE_APPEND;
	if (current_tkn.type == OUTFILE_TRUNC)
		save = OUTFILE_TRUNC;
	return (save);
}
