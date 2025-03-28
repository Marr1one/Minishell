/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:59:32 by maissat           #+#    #+#             */
/*   Updated: 2025/03/28 06:31:37 by braugust         ###   ########.fr       */
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

t_type case_infile(char *input, int i)
{
	int count ;

	count = 0;
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

t_type case_outfile(char *input, int i)
{
	int	count;

	count = 0;
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

t_type	case_redirect(char *input, int	i)
{
	if (input[i] == '<')
		return (case_infile(input, i));
	else
		return (case_outfile(input, i));
}
int is_word(char c, t_data *data)
{
	if (data->quote != 0)
	{
		if (c != data->quote)
			return (1);
		else
		{
			data->quote = -1;
			return (1);
		}
	}
	else if ((is_space(c) || is_redirect(c) || is_pipe(c)))
		return (0);
	return (1);
}

t_token *case_redir(char *input, int *i, t_data *data, t_token *list)
{
	t_type redirect;
	
	redirect = case_redirect(input, *i);
	if (redirect == UNKNOWN)
	{
        printf("erreur de redirect\n");
        while (input[*i] && (input[*i] == '<' || input[*i] == '>'))
        {
            (*i)++;
        }
        return (list);
	}
	if (input[*i] == '<')
		list = add_node("<", list, redirect);
	else
		list = add_node(">", list, redirect);
	if (redirect == INFILE || redirect == OUTFILE_TRUNC)
		(*i)++;
	else if (redirect == HEREDOC || redirect == OUTFILE_APPEND)
		*i += 2;
	data->expect = FICHIER;
	return (list);
}

t_token	*case_word(int *i, t_data *data, t_token *list, char *input)
{
	int	start;
	
	start = *i;
	while (input[*i] && is_word(input[*i], data) == 1) //"salut les gars"youpi youpo
	{
		if ((input[*i] == '"' || input[*i] == '\'') && data->quote == 0)
			data->quote = input[*i];
		if (data->quote == -1)
			data->quote = 0;
		(*i)++;
	}
	list = add_node(ft_substr_qte(input, start, *i - start), list, data->expect);
	if (data->expect == CMD)
		data->expect = ARG;
	if (data->expect == FICHIER)
		data->expect = ARG;
	return (list);
}

t_token *input_pipe(int *i, char *input, t_data *data, t_token *list)
{
	if (case_pipe(input, *i) > 1)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (NULL);
	}
	list = add_node("|", list, PIPE);
	data->expect = CMD;
	(*i)++;
	return (list);
}

// t_token	*tokenizer(char *input, t_data *data)
// {
// 	int			i;
// 	t_token		*list;

// 	i = 0;
// 	data->quote = 0;
// 	data->expect = CMD;
// 	list = NULL;
// 	if (!validate_input(input))
// 		return(NULL);
// 	while (input[i])
// 	{
// 		while (input[i] == ' ')
// 			i++;
// 		if (input[i] == '\0')
// 			break;
// 		if (is_word(input[i], data) == 1)
//         {
// 			list = case_word(&i, data, list, input);
//             continue;
//         }
// 		if (input[i] == '|')
// 		{
// 			list = input_pipe(&i, input, data, list);
// 			continue;
// 		}
// 		if (input[i] == '<' || input[i] == '>')
// 		{
// 			list = case_redir(input, &i, data, list);
// 			continue;
// 		}
// 		if (list && findlast_token(list)->type == PIPE)
// 		{
// 			printf("Pipe not closed\n");
// 			return (NULL);
// 		}
// 		i++;
// 	}
// 	return (list);
// }

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

// t_token *tokenizer(char *input, t_data *data)
// {
//     int i;
//     t_token *list;

//     data->quote = 0;
//     data->expect = CMD;
// 	i = 0;
// 	list = NULL;
//     if (!validate_input(input))
//         return (NULL);
//     while (input[i])
//     {
//         while (input[i] == ' ')
//             i++;
//         if (input[i] == '\0')
//             break;
//         if (is_word(input[i], data) == 1)
//             list = case_word(&i, data, list, input);
//         else if (input[i] == '|')
//             list = input_pipe(&i, input, data, list);
//         else if (input[i] == '<' || input[i] == '>')
//             list = case_redir(input, &i, data, list);
//         else if (list && findlast_token(list)->type == PIPE)
//         {
//             printf("Pipe not closed\n");
//             return (NULL);
//         }
//         else
//             i++;
//     }
//     return (list);
// }

t_token *handle_token_cases(char *input, int *i, t_data *data, t_token *list)
{
    if (is_word(input[*i], data))
	{
		list = case_word(i, data, list, input);
		return (list);
	}
    if (input[*i] == '|')
	{
		list = input_pipe(i, input, data, list);
        return (list);
	}
    if (input[*i] == '<' || input[*i] == '>')
	{
        list = case_redir(input, i, data, list);
		return (list);
	}
    return (list);
}

t_token *tokenizer(char *input, t_data *data)
{
    int i;
    t_token *list;

    data->quote = 0;
    data->expect = CMD;
	i = 0;
	list = NULL;
    if (!validate_input(input))
        return NULL;
    while (input[i])
    {
        while (input[i] == ' ')
            i++;
        if (input[i] == '\0')
            break;
        list = handle_token_cases(input, &i, data, list);
    }
	if (list && findlast_token(list)->type == PIPE)
        {
            printf("Pipe not closed\n");
            return (NULL);
        }
    return list;
}
