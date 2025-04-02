/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 13:19:59 by maissat           #+#    #+#             */
/*   Updated: 2025/04/02 22:47:28 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_numeric(char	*str)
{
	int	i;

	i = 0;
	if (str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_change(t_data *data, char *str)
{
	char 	**env;
	int		i;
	int		len;
	int		found;
	char	*name;

		
	name = take_before(str, '=');
	len = ft_strlen(name) + 1;
	env = data->envp;
	found = 0;
	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], ft_joinchar(name, '=', 0), len) == 0)
		{
			env[i] = ft_strdup(str);
			found = 1;
		}
		i++;
	}
	return (found);
}

char	**add_export(t_data *data, char *str)
{
	int	i;
	char	**new_env;

	i = 0;
	while (data->envp[i])
		i++;
	new_env = ft_malloc(sizeof(char *) * (i + 2));
	i = 0;
	while(data->envp[i])
	{
		new_env[i] = ft_strdup(data->envp[i]);
		i++;
	}
	new_env[i] = str;
	i++;
	new_env[i] = NULL;
	return (new_env);
}

char	**ft_unset(t_data *data, int save)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	while (data->envp[i])
		i++;
	new_env = ft_malloc(sizeof(char *) * i);
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while(data->envp[i])
	{
		if (i != save)
		{
			new_env[j] = ft_strdup(data->envp[i]);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}


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
			break;
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
