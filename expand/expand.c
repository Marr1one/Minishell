/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:14:44 by braugust          #+#    #+#             */
/*   Updated: 2025/04/09 16:09:31 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_dollar_counting(char *result, const char *input, t_idx *idx,
		int *dollar_count)
{
	int	k;

	*dollar_count = 0;
	while (input[*(idx->i)] == '$')
	{
		(*dollar_count)++;
		(*(idx->i))++;
	}
	k = -1;
	while (++k < *dollar_count / 2)
	{
		result[*(idx->j)] = '$';
		(*(idx->j))++;
	}
}

void	handle_exit_or_dollar(char *result, const char *input, t_data *data,
		t_idx *idx)
{
	char	*exit_str;
	int		k;

	if (input[*(idx->i)] == '?')
	{
		exit_str = ft_itoa(data->exit_status);
		k = 0;
		while (exit_str[k])
		{
			result[*(idx->j)] = exit_str[k];
			(*(idx->j))++;
			k++;
		}
		(*(idx->i))++;
	}
	else
	{
		result[*(idx->j)] = '$';
		(*(idx->j))++;
	}
}

void	process_variable_value(char *result, char *var_value, int *j)
{
	int	k;

	if (!var_value)
		var_value = "";
	k = 0;
	while (var_value[k])
	{
		result[*j] = var_value[k];
		(*j)++;
		k++;
	}
}

void	process_dollar_and_variables(char *result, const char *input,
		t_data *data, t_idx *idx)
{
	int		dollar_count;
	int		start;
	int		var_len;
	char	*var_name;
	char	*var_value;

	handle_dollar_counting(result, input, idx, &dollar_count);
	if (dollar_count % 2 == 0)
		return ;
	if (input[*(idx->i)] == '?' || !(input[*(idx->i)]
			&& (isalpha((unsigned char)input[*(idx->i)])
				|| input[*(idx->i)] == '_')))
	{
		handle_exit_or_dollar(result, input, data, idx);
		return ;
	}
	start = *(idx->i);
	while (input[*(idx->i)] && (isalnum((unsigned char)input[*(idx->i)])
			|| input[*(idx->i)] == '_'))
		(*(idx->i))++;
	var_len = *(idx->i) - start;
	var_name = ft_strndup(input + start, var_len);
	var_value = check_env(data, var_name);
	process_variable_value(result, var_value, idx->j);
}

void	process_regular_char(char *result, const char *input, t_data *data,
		t_idx *idx)
{
	if (input[*(idx->i)] == '\'' || input[*(idx->i)] == '"')
	{
		handle_quotes(input[*(idx->i)], data);
		result[*(idx->j)] = input[*(idx->i)];
		(*(idx->i))++;
		(*(idx->j))++;
		return ;
	}
	if (input[*(idx->i)] == '$' && data->in_quote != 2)
	{
		process_dollar_and_variables(result, input, data, idx);
		return ;
	}
	result[*(idx->j)] = input[*(idx->i)];
	(*(idx->i))++;
	(*(idx->j))++;
}
