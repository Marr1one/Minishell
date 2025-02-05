/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 13:19:59 by maissat           #+#    #+#             */
/*   Updated: 2025/02/05 19:49:02 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_cd(t_data data)
{
	if (chdir(data.args[1]) != 0 )
		perror("cd");
}

void	ft_exit(void)
{
	exit(0);
}

void ft_echo(t_data data)
{
	int	i;
	int	n;
	
	i = 1;
	n = 0;
	if (ft_strlcmp(data.args[1], "-n") == 0)
	{
		n = 1;
		i++;
	}
	while (data.args[i])
	{
		printf("%s", data.args[i]);
		if (data.args[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (n == 0)
		printf("\n");
}

t_env	*find_last_node(t_env	*list)
{
	if (!list)
		return (NULL);
	while (list->next != NULL)
		list = list->next;	
	return (list);
}

void	show_list(t_env *list)
{
	while (list)
	{
		printf("%s=%s\n", list->name, list->value);
		list = list->next;
	}
}

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

char	*ft_strdupenv(char	*str, int start, int	i)
{
	int		k;
	char	*dup;

	k = 0;
	//printf("str[start] = %c, et str[i] (fin) = %c\n", str[start], str[i]);
	dup = malloc(sizeof(char) * (i - start) + 1);
	if (!dup)
    	return (NULL);
	while (start < i)
	{
		dup[k] = str[start];
		k++;
		start++;
	}
	dup[k] = '\0';
	return (dup);
}

char	*get_name(char	*str)
{
	int	i;
	char *name;

	i = 0;
	while(str[i] !=  '=')
		i++;
	name = ft_strdupenv(str, 0, i);
	return (name);
}

char	*get_value(char	*str)
{
	int		i;
	int		save;
	char	*value;

	i = 0;
	while (str[i] != '=')
		i++;
	i ++;
	save = i;
	while(str[i] !=  '\0')
		i++;
	value = ft_strdupenv(str, save, i);
	return (value);
}

int	fill_export(t_env *var,char	*str)
{
	int		i;
	int		save;
	int		quote;
	
	i = 0;
	quote = 0;
	while (is_alpha(str[i]) == 0)
		i++;
	if (str[i] == '=')
	{
		var->name = ft_strdupenv(str, 0, i);
		i++;
		save = i;
	}
	else 
		return (1);
	if (str[save] == '"')
		quote = 1;
	i++;
	while (is_alpha(str[i]) == 0)
		i++;
	if (str[i] == '\0')
	{
		printf("in!\n");
		var->value = ft_strdupenv(str, save, i);
		return (0);
	}
	else if ((quote == 1 && str[i] == '"'))
	{
		printf("in guillemet!\n");
		var->value = ft_strdupenv(str, save + 1, i);
		return (0);
	}
	else 
		return (1);
}

void	add_node_queue(t_env	*list)
{
	t_env	*new_node;
	
	while(list->next != NULL)
		list = list->next;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return;
	new_node->name = NULL;
	new_node->next = NULL;
	new_node->value = NULL;
}

void	fill_env(t_data *data)
{
	int	i;
	t_env *new_node;
	t_env *list;

	list = data->env_var;
	i = 0;
	while(data->envp[i])
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return ;
		new_node->name = get_name(data->envp[i]);
		new_node->value = get_value(data->envp[i]);
		new_node->next = NULL;
		if (!list)
			list = new_node;
		else
			find_last_node(list)->next = new_node;
		i++;
	}
	data->env_var = list;
	//show_list(data->env_var);
}

char	**add_export(t_data *data, char *str)
{
	int	i;
	char	**new_env;

	i = 0;
	while (data->envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while(data->envp[i])
	{
		new_env[i] = ft_strdup(data->envp[i]);
		i++;
	}
	new_env[i] = ft_strdup(str);
	i++;
	new_env[i] = NULL;
	return (new_env);
}

int	check_builtin(t_data *data)
{
	//t_env	*last;
	if (ft_strlcmp(data->args[0], "exit") == 0 ||ft_strlcmp(data->args[0], "\"exit\"") == 0)
	{
		ft_exit();
		return (1);
	}
	if (ft_strlcmp(data->args[0], "echo") == 0 ||ft_strlcmp(data->args[0], "\"echo\"") == 0)
	{
		ft_echo(*data);
		return (1);
	}
	if (ft_strlcmp(data->args[0], "cd") == 0 ||ft_strlcmp(data->args[0], "\"cd\"") == 0)
	{
		ft_cd(*data);
		return (1);
	}
	if (ft_strlcmp(data->args[0], "env") == 0 ||ft_strlcmp(data->args[0], "\"env\"") == 0)
	{
		//if(!data.env_var)
		//	fill_env(&data);
		show_tab(data->envp);
		return (1);
	}
	if (ft_strlcmp(data->args[0], "export") == 0 ||ft_strlcmp(data->args[0], "\"export\"") == 0)
	{
		printf("in export!\n");
		data->list = data->list->next;
		while(data->list != NULL)
		{
			printf("{%s}\n", data->list->content);
			if (check_export_compatibility(data->list->content) == 0)
				data->envp = add_export(data, data->list->content);
			else
				printf("Bad format!\n");
			data->list = data->list->next;
		}
		////show_tab(data.args);
		//fill_env(&data);
		//if (data.args[1])
		//{
		//printf("IN\n");
		// add_node_queue(data.env_var);
		// last = find_last_node(data.env_var);
		// if (fill_export(last, data.args[1]) == 1)
		// 	printf("Bad format\n");
		//else
		//{
		//	printf("Good format!\n");
		//	show_list(data.env_var);
		//}
		//}
		return (1);
	}
	return (0);
}
//if (check_env_var(&data) == 0)
//		{
//			printf("Good format\n");
//			printf("NAME = %s\n", data.env_var->name);
//			printf("VALUE = %s\n", data.env_var->value);
//		}
//		else
//		{
//			{
//				printf("Bad format\n");
//				printf("NAME = %s\n", data.env_var->name);
//				printf("VALUE = %s\n", data.env_var->value);
//			}