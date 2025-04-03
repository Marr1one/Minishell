/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_after.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:28:12 by braugust          #+#    #+#             */
/*   Updated: 2025/04/04 00:17:41 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_char_index(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

static int	get_length_from(char *str, int start)
{
	int	len;

	len = 0;
	while (str[start])
	{
		len++;
		start++;
	}
	return (len);
}

static void	fill_after(char *dest, char *src, int start)
{
	int	i;

	i = 0;
	while (src[start])
	{
		dest[i] = src[start];
		i++;
		start++;
	}
	dest[i] = '\0';
}

char	*take_after(char *str, char c)
{
	char	*res;
	int		index;
	int		len;

	index = get_char_index(str, c);
	if (str[index] != c)
		return (NULL);
	len = get_length_from(str, index + 1);
	res = ft_malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	fill_after(res, str, index + 1);
	return (res);
}

char	*take_before(char *str, char c)
{
	char	*res;
	int		i;
	int		len;

	i = 0;
	while (str[i] != c)
		i++;
	len = i;
	res = ft_malloc(sizeof(char) * (len + 1));
	i = 0;
	while (i < len)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
