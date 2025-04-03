/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:17:40 by maissat           #+#    #+#             */
/*   Updated: 2025/04/03 19:53:55 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_arguments(t_token *list)
{
	int		count;
	t_token	*current;

	count = 0;
	if (!list)
		return (0);
	if (list->type == PIPE)
		current = list->next;
	else
		current = list;
	while (current)
	{
		if (current->type == PIPE)
			break ;
		if (current->type == CMD || current->type == ARG)
			count++;
		current = current->next;
	}
	return (count);
}

t_cmd	*create_args(t_token *lst_tkn, t_cmd *lst_cmd)
{
	int		i;
	t_token	*curr_tkn;
	t_cmd	*curr_cmd;

	curr_tkn = lst_tkn;
	curr_cmd = lst_cmd;
	i = 0;
	curr_cmd->args = ft_malloc(sizeof(char *) * \
		(count_arguments(curr_tkn) + 1));
	while (curr_tkn && curr_cmd)
	{
		if (curr_tkn->type == CMD || curr_tkn->type == ARG)
			curr_cmd->args[(i)++] = curr_tkn->content;
		else if (curr_tkn->type == PIPE)
		{
			curr_cmd->args[i] = NULL;
			i = 0;
			curr_cmd = curr_cmd->next;
			curr_cmd->args = ft_malloc(sizeof(char *) * \
				(count_arguments(curr_tkn) + 1));
		}
		curr_tkn = curr_tkn->next;
	}
	if (curr_cmd)
		curr_cmd->args[i] = NULL;
	return (lst_cmd);
}

t_cmd	*add_cmd_node(t_cmd *cmd_list)
{
	t_cmd	*new_cmd;
	t_cmd	*last;

	new_cmd = ft_malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	ft_memset(new_cmd, 0, sizeof(t_cmd));
	if (!cmd_list)
		return (new_cmd);
	else
	{
		last = cmd_list;
		while (last->next)
			last = last->next;
		last->next = new_cmd;
	}
	return (cmd_list);
}

t_cmd	*parse_cmd(t_token *list)
{
	t_token	*current;
	t_cmd	*list_cmd;

	list_cmd = NULL;
	list_cmd = add_cmd_node(list_cmd);
	current = list;
	while (current)
	{
		if (current->type == PIPE)
		{
			if (!current->next || current->next->type == PIPE)
			{
				printf("Syntax error: pipe not closed\n");
				return (NULL);
			}
			list_cmd = add_cmd_node(list_cmd);
		}
		current = current->next;
	}
	return (list_cmd);
}

t_cmd	*create_files(t_token *list_tkn, t_cmd *list_cmd)
{
	t_token	*current_tkn;
	t_cmd	*current_cmd;
	t_type	save;

	current_tkn = list_tkn;
	save = UNKNOWN;
	current_cmd = list_cmd;
	while (current_tkn && current_cmd)
	{
		if (is_redirect(current_tkn->content[0]))
			save = save_mode(*current_tkn);
		if (current_tkn->type == FICHIER)
		{
			current_cmd->files = add_file(current_cmd->files, \
				current_tkn, save);
			save = UNKNOWN;
		}
		if (current_tkn->type == PIPE)
		{
			current_cmd = current_cmd->next;
			save = UNKNOWN;
		}
		current_tkn = current_tkn->next;
	}
	return (list_cmd);
}
