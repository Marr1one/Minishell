/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:08:35 by braugust          #+#    #+#             */
/*   Updated: 2025/04/03 23:56:23 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_joinunset(char *str)
{
	int		i;
	int		j;
	int		len;
	char	*joineg;

	len = ft_strlen(str);
	i = 0;
	j = 0;
	joineg = ft_malloc(sizeof(char) * (len + 2));
	if (!joineg)
		return (NULL);
	while (str[i])
	{
		joineg[j] = str[i];
		j++;
		i++;
	}
	joineg[j++] = '=';
	joineg[j] = '\0';
	return (joineg);
}

void	check_unset(t_data *data, char	*str)
{
	int		i;
	char	*join_eg;

	i = 0;
	join_eg = ft_joinunset(str);
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], join_eg, ft_strlen(join_eg)) == 0)
		{
			data->envp = ft_unset(data, i);
			break ;
		}
		i++;
	}
	data->exit_status = 0;
}

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}
