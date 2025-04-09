/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 21:02:27 by braugust          #+#    #+#             */
/*   Updated: 2025/04/09 16:33:39 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_single_file(t_file *file)
{
	int	fd;

	if (file->mode == HEREDOC)
		return (0);
	fd = open_file(file->path, file->mode);
	if (fd == -1)
	{
		printf("open");
		return (1);
	}
	if (redirect_fd(fd, file->mode))
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	open_file(char *path, t_type mode)
{
	int	fd;

	fd = -1;
	if (mode == INFILE)
		fd = open(path, O_RDONLY);
	else if (mode == OUTFILE_TRUNC)
		fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (mode == OUTFILE_APPEND)
		fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

int	redirect_fd(int fd, int mode)
{
	if (mode == INFILE || mode == HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			return (1);
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (1);
	}
	return (0);
}

int	handle_file_redirections(t_cmd *current_cmd)
{
	t_file	*current_file;

	if (!current_cmd->files)
		return (0);
	current_file = current_cmd->files;
	while (current_file)
	{
		if (handle_single_file(current_file))
			return (1);
		current_file = current_file->next;
	}
	return (0);
}
