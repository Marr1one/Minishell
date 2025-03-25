/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:15:10 by maissat           #+#    #+#             */
/*   Updated: 2025/03/25 16:54:24 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_global_quotes(char *str)
{
	int		i;
	int		count;
	
	i = 0;
	count = 0;
	
	while (str[i])
	{
		
		if (str[i] == '"' || str[i] == '\'')
			count++;
		i++;
	}
	return (count);
}

// char	*rm_qts_str(char *str)
// {
// 	int	i;
// 	int	j;
// 	int	nb_quotes = count_global_quotes(str);
// 	char	*new_str;

// 	i = 0;
// 	while (str[i])
// 		i++;
// 	new_str = ft_malloc (sizeof(char) * (i - nb_quotes + 1));
// 	if (!new_str)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	while (str[i])
// 	{
// 		if (str[i] != '"' && str[i] != '\'')
// 		{
// 			new_str[j] = str[i];
// 			i++;
// 			j++;
// 		}
// 		else
// 			i++;
// 	}
// 	new_str[j] = '\0';
// 	return (new_str);
// }

// void	rm_qts_nodes(t_data *data)
// {
// 	t_token	*current;

// 	current = data->list;
// 	while (current)
// 	{
// 		current->content = rm_qts_str(current->content);
// 		current = current->next;
// 	}		
// }

int handle_quotes(char c, t_data *data)
{
    if (c == '"' && data->in_quote != 2)
    {
        if (data->in_quote == 1)
            data->in_quote = 0;
        else
            data->in_quote = 1;
        return (1);
    }
    else if (c == '\'' && data->in_quote != 1)
    {
        if (data->in_quote == 2)
            data->in_quote = 0;
        else
            data->in_quote = 2;
        return (1);
    }
    return (0);
}
char *rm_qts_str(char *str, t_data *data)
{
    int i;
    int j;
    char *new_str;

    /* Réinitialiser l'état des quotes pour cette chaîne */
    data->in_quote = 0;
    new_str = ft_malloc(sizeof(char) * (ft_strlen(str) + 1));
    if (!new_str)
        return (NULL);
    i = 0;
    j = 0;
    while (str[i])
    {
        if (handle_quotes(str[i], data))
        {
            i++;
            continue;
        }
        new_str[j++] = str[i++];
    }
    new_str[j] = '\0';
    return (new_str);
}

void rm_qts_nodes(t_data *data)
{
    t_token *current;

    current = data->list;
    while (current)
    {
        current->content = rm_qts_str(current->content, data);
        current = current->next;
    }
}
