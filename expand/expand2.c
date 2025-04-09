/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:11:17 by braugust          #+#    #+#             */
/*   Updated: 2025/04/09 16:09:40 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_redirect(char c)
{
	if (c == '>' || c == '<')
		return (1);
	return (0);
}

int	is_pipe(char c)
{
	if (c == '|')
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

void	build_expanded_string(char *result, const char *input, t_data *data)
{
	int		i;
	int		j;
	t_idx	idx;

	i = 0;
	j = 0;
	idx.i = &i;
	idx.j = &j;
	data->in_quote = 0;
	while (input[i])
	{
		process_regular_char(result, input, data, &idx);
	}
	result[j] = '\0';
}

char	*expand_string(const char *input, t_data *data)
{
	char	*result;
	int		final_len;

	if (!input)
		return (NULL);
	final_len = calc_expanded_len(input, data);
	result = ft_malloc(final_len + 1);
	if (!result)
		return (NULL);
	build_expanded_string(result, input, data);
	return (result);
}
