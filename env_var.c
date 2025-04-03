/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 19:39:51 by maissat           #+#    #+#             */
/*   Updated: 2025/04/03 23:55:54 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_joinchar(char *str, char c, int x)
{
	int		i;
	int		j;
	int		len;
	char	*join;

	len = ft_strlen(str);
	if (x == 1)
		i = 1;
	else
		i = 0;
	j = 0;
	join = ft_malloc(sizeof(char) * (len + 2));
	if (!join)
		return (NULL);
	while (str[i])
	{
		join[j] = str[i];
		j++;
		i++;
	}
	join[j] = c;
	j++;
	join[j] = '\0';
	return (join);
}

int	index_match(t_data *data, char *str)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str) + 1;
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], ft_joinchar(str, '=', 1), len) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	*check_env(t_data *data, char *str)
{
	int		i;
	int		len;
	char	*search_str;

	i = 0;
	if (str[0] == '$')
		search_str = str + 1;
	else
		search_str = str;
	len = ft_strlen(search_str);
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], search_str, len) == 0
			&& data->envp[i][len] == '=')
			return (take_after(data->envp[i], '='));
		i++;
	}
	return (NULL);
}
