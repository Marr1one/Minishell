/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chaining.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:45:04 by maissat           #+#    #+#             */
/*   Updated: 2025/03/11 18:26:41 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//int	count_args(char **tab)
//{
//	int	i;

//	i = 0;
//	while (tab[i])
//		i++;
//	return (i);
//}

//void	show_content(t_token *token)
//{
//	t_token *current;

//	current = token;
//	while (current != NULL)
//	{
//		printf("token %d content : %s, type ;%d\n", current->index,current->content, current->type);
//		current = current->next;
//	}
//}

t_token	*findlast_token(t_token	*list)
{
	if (!list)
		return (NULL);
	while (list->next != NULL)
		list = list->next;	
	return (list);
}


int	is_arg(char *str)
{
	int	i;
	int	check;

	check = 0;
	i = 0;
	while (str[i])
	{
		if (str[0] == '-')
		{
			if (str[1] == '\0')
				return (1);
			else
				check =  1;
		}
		i++;
	}
	if (check == 1)
		return (0);
	return (1);
	
}

void	check_redirect(t_token *current)
{
	if (ft_strlcmp(current->content, "<<") == 0)
		return (current->type = HEREDOC, 1);
	else if (ft_strlcmp(current->content, ">") == 0)
		return (current->type = OUTFILE_TRUNC, 1);
	else if (ft_strlcmp(current->content, "<") == 0)
		return (current->type = INFILE, 1);
	else if (ft_strlcmp(current->content, ">>") == 0)
		return (current->type = OUTFILE_APPEND, 1);
	
}

t_token	*add_chained_list(char **tab)
{
	int	i;
	t_token	*head;
	t_token *prev;
	t_token *current;


	head = NULL;
	current = NULL;
	prev = NULL;
	i = 0;
	while(tab[i])
	{
		current = ft_malloc(sizeof(t_token));
		if (!current)
			return (NULL);
		current->content = ft_strdup(tab[i]);
		if (ft_strlcmp(current->content, "|") == 0)
			current->type = PIPE;
		else if (ft_strlcmp(current->content, "<<") == 0)
			current->type = HEREDOC;
		else if (ft_strlcmp(current->content, ">") == 0)
			current->type = OUTFILE_TRUNC;
		else if (ft_strlcmp(current->content, "<") == 0)
			current->type = INFILE;
		else if (ft_strlcmp(current->content, ">>") == 0)
			current->type = OUTFILE_APPEND;
		else if(prev && prev->content )
		else //si celui davant est une redirection, le token apres est un file
			current->type = WORD;
		current->next = NULL;
		current->prev = prev;
		if (!head)
			head = current;
		else
		{
			prev->next = current;
			findlast_token(head)->next = current;
		}
		prev = current;
		i++;
	}
	//show_content(list);
	return (head);
}
