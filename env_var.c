/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 19:39:51 by maissat           #+#    #+#             */
/*   Updated: 2025/02/06 20:42:26 by maissat          ###   ########.fr       */
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

int	check_env(t_data *data, char *str)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str);
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], str, len) == 0)
		i++;
	}
}

int	check_dollar(t_data *data)
{
	t_token *list;

	list = data->list;
	while (list != NULL)
	{
		if (list->content[0] == '$')
			check_env(data, list->content);
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