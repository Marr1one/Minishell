/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:26:21 by maissat           #+#    #+#             */
/*   Updated: 2025/02/03 18:35:59 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_utils(char *str)
{
	int		i;
	int		j;
	char	*new_str;
	
	new_str = malloc(sizeof(char) * ft_strlen(str));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '"')
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
	
}

char	*remove_quotes(char *str)
{
	int		len;
	char	*new_str;
	
	len = ft_strlen(str);
	//printf("str[0] == %c et str[len - 1] == %c\n", str[0], str[len - 1]);
	if (str[0] == '"' && str[len - 1] == '"')
	{
		new_str = remove_utils(str);
		free(str);
		return (new_str);
	}
	return (str);
}

void	remove_quotes_all(t_data *data)
{
	int	i;

	i = 0;
	while (data->args[i])
	{
		data->args[i] = remove_quotes(data->args[i]);
		i++;
	}
		
}

int	ft_strlen(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_strncmp(char *s1, char *s2, int n)
{
	int	i;

	i = 0;
	if (!s1 || !s2 || n < 0)
		return (-1);
	while (s1[i] && s2[i] && i < n)
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	return (0);
}

int	check_len(char *s1, char *s2)
{
	int len_s1;
	int	len_s2;
	
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	if (len_s1 != len_s2)
		return (1);
	return (0);
}

int	ft_strlcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	if (!(*s1) || !(*s2))
		return (1);
	if (check_len(s1, s2) != 0)
		return (1);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	if (!(*s1) || !(*s2))
		return (1);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	return (0);
}

char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

void	*ft_memset(void *b, int c, size_t len)
{
	char	*start;

	if (b == NULL)
		return (NULL);
	start = b;
	while (len)
	{
		*start = c;
		start++;
		len--;
	}
	return (b);
}

char	*ft_strdup(char	*str)
{
	int		i;
	char	*dup;

	i = 0;
	dup = malloc(sizeof(char) * ft_strlen(str));
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
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
	join = malloc(sizeof(char) * ft_strlen(str) + ft_strlen(add) + 1);
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

