/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:59:32 by maissat           #+#    #+#             */
/*   Updated: 2025/04/07 20:00:49 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_word(char c, t_data *data)
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

t_token	*case_redir(char *input, int *i, t_data *data, t_token *list)
{
	t_type	redirect;
	char	save_redirect;

	save_redirect = input[*i];
	redirect = case_redirect(input, *i);
	if (data->expect == FICHIER)
	{
		printf("minishell: syntax error near unexpected"
			"token `%c'\n", save_redirect);
		return (NULL);
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
	while (input[*i] && is_word(input[*i], data) == 1)
	{
		if ((input[*i] == '"' || input[*i] == '\'') && data->quote == 0)
			data->quote = input[*i];
		if (data->quote == -1)
			data->quote = 0;
		(*i)++;
	}
	list = add_node(ft_substr_qte(input, start, *i - start), list,
			data->expect);
	if (data->expect == CMD)
		data->expect = ARG;
	if (data->expect == FICHIER)
		data->expect = ARG;
	return (list);
}

t_token	*input_pipe(int *i, char *input, t_data *data, t_token *list)
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

int	validate_input(const char *input)
{
	char	quote;
	int		i;

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
				return (printf("minishell :special character '%c'"
						"detected\n", input[i]), 0);
		}
		i++;
	}
	if (quote != 0)
		return (printf("minishell :quote not closed\n"), 0);
	return (1);
}
