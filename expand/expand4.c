/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:13:09 by braugust          #+#    #+#             */
/*   Updated: 2025/04/09 16:09:14 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	size_t	len;
	char	*dup;

	len = 0;
	while (s[len] && len < n)
		len++;
	dup = ft_malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int	handle_variable_expansion(const char *input, t_data *data, int *i)
{
	int		start;
	int		var_len;
	char	*var_name;
	char	*var_value;
	int		len;

	start = *i;
	while (input[*i] && (isalnum((unsigned char)input[*i]) || input[*i] == '_'))
	{
		(*i)++;
	}
	var_len = *i - start;
	var_name = ft_strndup(input + start, var_len);
	var_value = check_env(data, var_name);
	if (!var_value)
		var_value = "";
	len = strlen(var_value);
	return (len);
}

int	count_dollar_signs(const char *input, int *i)
{
	int	dollar_count;
	int	len;

	dollar_count = 0;
	len = 0;
	while (input[*i] == '$')
	{
		dollar_count++;
		(*i)++;
	}
	len += dollar_count / 2;
	return (dollar_count);
}

int	process_dollar_expansion(const char *input, t_data *data, int *i)
{
	int		dollar_count;
	int		len;
	char	*exit_str;

	len = 0;
	dollar_count = count_dollar_signs(input, i);
	len += dollar_count / 2;
	if (dollar_count % 2 == 0)
		return (len);
	if (input[*i] == '?')
	{
		exit_str = ft_itoa(data->exit_status);
		len += strlen(exit_str);
		(*i)++;
	}
	else if (input[*i] && (isalpha((unsigned char)input[*i])
			|| input[*i] == '_'))
		len += handle_variable_expansion(input, data, i);
	else
		len++;
	return (len);
}

int	calc_expanded_len(const char *input, t_data *data)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	data->in_quote = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			handle_quotes(input[i], data);
			len++;
			i++;
			continue ;
		}
		if (input[i] == '$' && data->in_quote != 2)
		{
			len += process_dollar_expansion(input, data, &i);
			continue ;
		}
		len++;
		i++;
	}
	return (len);
}
