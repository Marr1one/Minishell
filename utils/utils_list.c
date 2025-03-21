/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 16:10:49 by maissat           #+#    #+#             */
/*   Updated: 2025/03/21 17:43:44 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**list_to_args(t_data *data)
{
	t_token *current;
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
	int	i;
	t_token *current;
	
	i = 0;
	current = list;
	while (current)
	{
		current = current->next;
		i++;
	}
	return (i);
}


int	get_nbr_node(t_token *list)
{
	int	i;
	t_token *current;
	
	i = 0;
	current = list;
	while (current != NULL)
	{
		i++;
		current = current->next;
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
	dest = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
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

char	*ft_re_strjoin(char *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*dest;

	i = 0;
	j = 0;
	dest = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!dest)
	{
		return (NULL);
	}
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
	free(s1);
	return (dest);
}

char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	dlen;

	i = 0;
	dlen = ft_strlen(dest);
	while (src[i])
	{
		dest[dlen + i] = src[i];
		i++;
	}
	dest[dlen + i] = 0;
	return (dest);
}
