/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 19:39:51 by maissat           #+#    #+#             */
/*   Updated: 2025/02/27 02:38:53 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_alpha(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c<= 'Z'))
		return (0);
	else
		return (1);
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

char	*ft_joinchar(char *str, char c)
{
	int		i;
	int		j;
	int		len;
	char	*join;

	len = ft_strlen(str);
	i = 1;
	j = 0;
	join = ft_malloc(sizeof(char) * (len + 1));
	if (!join)
		return (NULL);
	while (str[i])
	{
		join[j] = str[i];
		j++;
		i++;
	}
	join[j] = c;
	i++;
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
		if (ft_strncmp(data->envp[i], ft_joinchar(str, '='), len) == 0)
		{
			// printf("dans index ; data.envp trouve = %s\n", data->envp[i]);
			return (i);
		}
		i++;
	}
	return (-1);
}

void	check_env(t_data *data, char *str, t_token *current)
{
	int	i;
	int	len;
	int	index;
	
	i = 0;
	index = 0;
	len = ft_strlen(str) + 1;
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], ft_joinchar(str, '='), len) == 0)
		{
			// printf("data.envp trouve = %s\n", data->envp[i]);
			index = index_match(data, str);
			// printf("data.envp[index] = %s\n", data->envp[index]);
			current->content = ft_strdup(take_after(data->envp[index], '='));
			printf("content : %s\n", current->content);
			return ;
		}
		i++;
	}
}

void	check_dollar(t_data *data)
{
	t_token *list;
	int		len;
	int		i;
	
	list = data->list;
	while (list != NULL)
	{
		i = 0;
		len = ft_strlen(list->content);
		while (list->content[i])
		{
			if (list->content[i] == '$')
			{
				printf("on trouve un $\n");
				if ((list->content[0] == '"' && list->content[len - 1] == '"'))
				{
					remove_quotes_all(data);
					//list = add_chained_list(data);
					//printf("list dans le cas la\n");
					//show_list(data->list);
					check_env(data, list->content, list);
				}
				else
					check_env(data, list->content, list);
			}
			i++;
		}
		list = list->next;
	}
}

//char	*ft_strdupenv(char	*str, int start, int	i)
//{
//	int		k;
//	char	*dup;

//	k = 0;
//	printf("str[start] = %c, et str[i] (fin) = %c\n", str[start], str[i]);
//	dup = malloc(sizeof(char) * (i - start) + 1);
//	if (!dup)
//    	return (NULL);
//	while (start < i)
//	{
//		dup[k] = str[start];
//		k++;
//		start++;
//	}
//	dup[k] = '\0';
//	return (dup);
//}

//int handle_env(t_data *data, char *str)
//{
//	int		i;
//	int		save;
//	int		quote;
	
//	i = 0;
//	quote = 0;
//	if (!data->env_var)
//	{
//		data->env_var = malloc(sizeof(t_env));
//		if (!data->env_var)
//			return (1);
//		data->env_var->name = NULL;
//		data->env_var->value = NULL;
//	}
    	
//	while (is_alpha(str[i]) == 0)
//		i++;
//	if (str[i] == '=')
//	{
//		data->env_var->name = ft_strdupenv(str, 0, i);
//		i++;
//		save = i;
//	} //ici il faut juste que jajoute ce quil ya dans jusqua i dans une case ; NAME
//	else 
//		return (1);
//	if (str[save] == '"')
//		quote = 1;
//	i++;
//	while (is_alpha(str[i]) == 0)
//		i++;
//	if (str[i] == '\0')
//	{
//		printf("in!\n");
//		data->env_var->value = ft_strdupenv(str, save, i);
//		return (0);
//	}
//	else if ((quote == 1 && str[i] == '"'))
//	{
//		printf("in guillemet!\n");
//		data->env_var->value = ft_strdupenv(str, save + 1, i);
//		return (0);
//	}  
//	//ici il faut juste que jajoute ce quil ya entre = et \0 dans une varaible VALUE
//	else 
//		return (1);
//}