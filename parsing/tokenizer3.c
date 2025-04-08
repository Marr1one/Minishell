/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:00:03 by braugust          #+#    #+#             */
/*   Updated: 2025/04/08 04:40:22 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*handle_token_cases(char *input, int *i, t_data *data, t_token *list)
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

t_token	*tokenizer(char *input, t_data *data)
{
	int		i;
	t_token	*list;

	data->quote = 0;
	data->expect = CMD;
	i = 0;
	list = NULL;
	if (!validate_input(input))
		return (NULL);
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (input[i] == '\0')
			break ;
		list = handle_token_cases(input, &i, data, list);
		if (list == NULL)
			return (NULL);
	}
	if (list && (findlast_token(list)->type == PIPE
			|| is_tkn_redir(findlast_token(list)) || 
			findfirst_token(list)->type == PIPE))
		return (printf("minishell: syntax error\n"), NULL);
	return (list);
}
