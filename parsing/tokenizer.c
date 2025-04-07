/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:59:32 by maissat           #+#    #+#             */
/*   Updated: 2025/04/07 18:59:40 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*add_node(char *str, t_token *list, t_type type)
{
	t_token	*new_node;

	new_node = ft_malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->next = NULL;
	new_node->content = ft_strdup(str);
	new_node->type = type;
	if (!list)
		list = new_node;
	else
		findlast_token(list)->next = new_node;
	return (list);
}

int	case_pipe(char *input, int i)
{	
	int	count;

	count = 0;
	while (input[i] == '|')
	{
		count++;
		i++;
	}
	return (count);
}

t_type	case_infile(char *input, int i)
{
	int	count;

	count = 0;
	while (input[i] == '<')
	{
		(i)++;
		count++;
	}
	if (count == 1)
		return (INFILE);
	if (count == 2)
		return (HEREDOC);
	return (UNKNOWN);
}

t_type	case_outfile(char *input, int i)
{
	int	count;

	count = 0;
	while (input[i] == '>')
	{
		(i)++;
		count++;
	}
	if (count == 1)
		return (OUTFILE_TRUNC);
	if (count == 2)
		return (OUTFILE_APPEND);
	return (UNKNOWN);
}

t_type	case_redirect(char *input, int i)
{
	if (input[i] == '<')
		return (case_infile(input, i));
	else
		return (case_outfile(input, i));
}
