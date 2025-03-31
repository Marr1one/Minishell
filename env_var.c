/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 19:39:51 by maissat           #+#    #+#             */
/*   Updated: 2025/03/31 20:05:55 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_alpha(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c<= 'Z'))
		return (1);
	else
		return (0);
}

int	is_alphanum(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c<= 'Z') ||
		(c >= '0' && c <= '9'))
		return (1);
	else
		return (0);
}

int	is_digit(char c)
{
	if ((c >= '0' && c <= '9'))
		return (1);
	else
		return (0);
}

char	*take_after(char *str, char c)
{
	int		i;
	int		len;
	int		save;
	char	*res;

	i = 0;
	while(str[i] != c)
		i++;
	save = i;
	len = 0;
	while (str[i])
	{
		i++;
		len++;
	}
	res = ft_malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	save++;
	i = 0;
	while(str[save])
	{
		res[i] = str[save];
		save++;
		i++;
	}
	res[i] = '\0';
	return (res);
}



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
		{
			// printf("dans index ; data.envp trouve = %s\n", data->envp[i]);
			return (i);
		}
		i++;
	}
	return (-1);
}

char *check_env(t_data *data, char *str)
{
    printf("dans check env\n");
    int i = 0;
    int len;
    
    // Si str commence par $, ignorer ce caractère pour la recherche
    char *search_str;
    if (str[0] == '$') {
        search_str = str + 1;  // Pointer après le $
    } else {
        search_str = str;
    }
    
    len = ft_strlen(search_str);
    
    while (data->envp[i])
    {
        // Comparer directement avec la variable d'environnement jusqu'au =
        if (ft_strncmp(data->envp[i], search_str, len) == 0 && data->envp[i][len] == '=')
        {
            return take_after(data->envp[i], '=');
        }
        i++;
    }
    return (NULL);
}