/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:59:32 by maissat           #+#    #+#             */
/*   Updated: 2025/03/21 18:29:28 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*add_node(char *str, t_token *list,  t_type type)
{
	t_token *new_node;

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
int	case_pipe(char *input, int	i)
{	
	int	count;
	
	count = 0;
	while(input[i] == '|')
	{
		count++;
		i++;
	}
	return (count);
	
}
t_type	case_redirect(char *input, int	i)
{
	int	count;
	
	count = 0;
	if (input[i] == '<')
	{
		while (input[i] == '<')
		{
			i++;
			count++;
		}
		if (count == 1)
			return (INFILE);
		if (count == 2)
			return (HEREDOC);
		return (UNKNOWN);
	}
	else
	{
		while (input[i] == '>')
		{
			i++;
			count++;
		}
		if (count == 1)
			return (OUTFILE_TRUNC);
		if (count == 2)
			return (OUTFILE_APPEND);
		return (UNKNOWN);
	}
}
int is_word(char c)
{
	if (is_space(c))
		return (0);
	if (is_redirect(c))
		return (0);
	if (is_pipe(c))
		return (0);
	return (1);
}

t_token	*tokenizer(char *input)
{
	int			i;
	t_token		*list;
	t_type		expect;
	t_type		redirect;
	//char		quote;
	int			start;

	i = 0;
	expect = CMD;
	list = NULL;
	if (!validate_input(input))
	{
		return(NULL);
	}
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (input[i] == '\0')
		{
			// printf("dans le cas ou il nya que des espaces!\n");
			break;
		}
		if (is_word(input[i]) == 1)
		{
			start = i;
			while(input[i] && is_word(input[i]) == 1)
			{
				i++;
			}
			list = add_node(ft_substr(input, start, i), list, expect);
			if (expect == CMD)
				expect = ARG;
			if (expect == FICHIER)
				expect = ARG;
			continue;
		}
		if (input[i] == '|')
		{
			if (case_pipe(input, i) > 1)
				break;
			list = add_node("|", list, PIPE);
			expect = CMD;
			i++;
			continue;
		}
		if (input[i] == '<' || input[i] == '>')
		{
			redirect = case_redirect(input, i);
			if (redirect == UNKNOWN)
			{
				printf("erreur de redirect\n");
				break;
			}
			if (input[i] == '<')
				list = add_node("<", list, redirect);
			else
				list = add_node(">", list, redirect);
			if (redirect == INFILE || redirect == OUTFILE_TRUNC)
                i++;
            else if (redirect == HEREDOC || redirect == OUTFILE_APPEND)
                i += 2;
			expect = FICHIER;
			continue;
		}
		if (list && findlast_token(list)->type == PIPE)
		{
			printf("Pipe not closed\n");
			return (NULL);
		}
		i++;
	}
	return (list);
}

int validate_input(const char *input)
{
    char quote;
    int i;

	i = 0;
	quote = 0;
    while (input[i])
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            if (quote == 0)
                quote = input[i];
            else if (quote == input[i])
                quote = 0;
        }
        else if (quote == 0)
        {
            if (input[i] == '\\' || input[i] == ';')
            {
                fprintf(stderr, "Erreur de parsing : caractère spécial\n");
                return (0);
			}
        }
        i++;
    }
    if (quote != 0)
    {
        fprintf(stderr, "Erreur de parsing : quote non fermée\n");
        return (0);
    }
    return (1);
}
