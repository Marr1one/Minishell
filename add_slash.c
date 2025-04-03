/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_slash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:46:48 by maissat           #+#    #+#             */
/*   Updated: 2025/04/03 23:56:00 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**add_slash_all(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return (NULL);
	while (tab[i])
	{
		tab[i] = add_slash(tab[i]);
		i++;
	}
	return (tab);
}

char	*ft_strcpy(char *str, char *dest)
{
	int		i;

	i = 0;
	if (!str || !dest)
		return (NULL);
	while (str[i])
	{
		dest[i] = str[i];
		i++;
	}
	return (dest);
}

char	*add_slash(char	*str)
{
	char	*new_str;

	if (!str)
		return (NULL);
	new_str = ft_malloc(sizeof(char) * (ft_strlen(str) + 2));
	if (!new_str)
		return (NULL);
	new_str = ft_strcpy(str, new_str);
	new_str[ft_strlen(str)] = '/';
	new_str[ft_strlen(str) + 1] = '\0';
	return (new_str);
}
