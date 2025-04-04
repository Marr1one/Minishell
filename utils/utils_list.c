/* ************************************************************************** */
/*						                                                    */
/*                                                        :::      ::::::::   */
/*   utils_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:10:49 by maissat           #+#    #+#             */
/*   Updated: 2025/04/04 15:47:19 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**list_to_args(t_data *data)
{
	t_token	*current;
	char	**new_args;
	int		i;

	i = 0;
	new_args = ft_malloc(sizeof(char *) * (list_len(data->list) + 1));
	if (!new_args)
		return (NULL);
	current = data->list;
	while (current)
	{
		new_args[i] = ft_strdup(current->content);
		i++;
		current = current->next;
	}
	new_args[i] = NULL;
	return (new_args);
}

int	list_len(t_token *list)
{
	int		i;
	t_token	*current;

	i = 0;
	current = list;
	while (current)
	{
		current = current->next;
		i++;
	}
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*dest;

	i = 0;
	j = 0;
	dest = ft_malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!dest)
		return (NULL);
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		dest[i + j] = s2[j];
		j++;
	}
	dest[j + i] = '\0';
	return (dest);
}

char	*ft_join(char *str, char *add)
{
	int		i;
	int		j;
	char	*join;

	i = 0;
	j = 0;
	if (!str || !add)
		return (NULL);
	join = ft_malloc(sizeof(char) * (ft_strlen(str) + ft_strlen(add) + 1));
	if (!join)
		return (NULL);
	while (str[i])
	{
		join[i] = str[i];
		i++;
	}
	j = 0;
	while (add[j])
	{
		join[i] = add[j];
		i++;
		j++;
	}
	join[i] = 0;
	return (join);
}

char	*expand_exit_status(t_data *data)
{
	char	*expanded;

	expanded = ft_itoa(data->exit_status);
	return (expanded);
}
