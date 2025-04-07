/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:14:55 by maissat           #+#    #+#             */
/*   Updated: 2025/04/07 15:54:19 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	calculate_length_without_quotes(char *str)
{
	int	i;
	int	j;
	int	in_dquote;
	int	in_squote;

	i = 0;
	j = 0;
	in_dquote = 0;
	in_squote = 0;
	while (str[i])
	{
		if (str[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		else if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else
			j++;
		i++;
	}
	return (j);
}

// Remplit la nouvelle chaîne sans les guillemets
char	*fill_string_without_quotes(char *new_str, char *str)
{
	int	i;
	int	j;
	int	in_dquote;
	int	in_squote;

	i = 0;
	j = 0;
	in_dquote = 0;
	in_squote = 0;
	while (str[i])
	{
		if (str[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		else if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else
			new_str[j++] = str[i];
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

//Fonction principale qui enlève les guillemets d'une chaîne
char	*quoteless_string(char *str)
{
	int		length;
	char	*new_str;

	length = calculate_length_without_quotes(str);
	new_str = ft_malloc(sizeof(char) * (length + 1));
	if (!new_str)
		return (NULL);
	new_str = fill_string_without_quotes(new_str, str);
	return (new_str);
}

void	remove_quotes(t_cmd *list_cmd)
{
	t_cmd	*current_cmd;
	int		i;

	i = 0;
	current_cmd = list_cmd;
	while (current_cmd)
	{
		i = 0;
		while (current_cmd->args && current_cmd->args[i])
		{
			current_cmd->args[i] = quoteless_string(current_cmd->args[i]);
			i++;
		}
		current_cmd = current_cmd->next;
	}
}

char	*quoteless_string_cmd(char *str)
{
	int	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 4 && str[0] == '"' && str[len - 1] == '"' && \
		str[1] == '\'' && str[len - 2] == '\'')
	{
		return (ft_substr(str, 1, len - 1));
	}
	if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'') || \
						(str[0] == '"' && str[len - 1] == '"')))
	{
		return (ft_substr(str, 1, len - 1));
	}
	return (ft_strdup(str));
}
