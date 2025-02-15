/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 13:19:59 by maissat           #+#    #+#             */
/*   Updated: 2025/02/15 19:46:45 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_cd(t_data *data)
{
	char	*old_pwd;

	old_pwd = getenv("OLDPWD");
	printf("old_pwd = %s\n", old_pwd);
	if (data->args[1] && ft_strlcmp(data->args[1], "-") == 0)
	{
		if (!old_pwd)
		{
			printf("minishell: cd: OLDPWD not set\n");
			data->exit_status = 1;
			return ;
		}
		printf("%s\n", old_pwd);
		if (chdir(old_pwd) != 0)
			perror("cd");
		data->exit_status = 0;
		return;
	}
	else if (data->args[1] && chdir(data->args[1]) != 0)
	{
		data->exit_status = 1;
		perror("cd");
	}
	else
		data->exit_status = 0;
}

void	ft_exit(t_data *data)
{
	int	status;

	status = 0;
	if (data->args[1])
	{
		status = ft_atoi(data->args[1]);
		if (status < 0 || status > 255)
			status = status % 256;
	}
	exit(status);
}


// char	*ft_strdupenv(char	*str, int start, int	i)
// {
// 	int		k;
// 	char	*dup;

// 	k = 0;
// 	//printf("str[start] = %c, et str[i] (fin) = %c\n", str[start], str[i]);
// 	dup = malloc(sizeof(char) * (i - start) + 1);
// 	if (!dup)
//     	return (NULL);
// 	while (start < i)
// 	{
// 		dup[k] = str[start];
// 		k++;
// 		start++;
// 	}
// 	dup[k] = '\0';
// 	return (dup);
// }

// int	fill_export(t_env *var,char	*str)
// {
// 	int		i;
// 	int		save;
// 	int		quote;
	
// 	i = 0;
// 	quote = 0;
// 	while (is_alpha(str[i]) == 0)
// 		i++;
// 	if (str[i] == '=')
// 	{
// 		var->name = ft_strdupenv(str, 0, i);
// 		i++;
// 		save = i;
// 	}
// 	else 
// 		return (1);
// 	if (str[save] == '"')
// 		quote = 1;
// 	i++;
// 	while (is_alpha(str[i]) == 0)
// 		i++;
// 	if (str[i] == '\0')
// 	{
// 		printf("in!\n");
// 		var->value = ft_strdupenv(str, save, i);
// 		return (0);
// 	}
// 	else if ((quote == 1 && str[i] == '"'))
// 	{
// 		printf("in guillemet!\n");
// 		var->value = ft_strdupenv(str, save + 1, i);
// 		return (0);
// 	}
// 	else 
// 		return (1);
// }

// void	add_node_queue(t_env	*list)
// {
// 	t_env	*new_node;
	
// 	while(list->next != NULL)
// 		list = list->next;
// 	new_node = malloc(sizeof(t_env));
// 	if (!new_node)
// 		return;
// 	new_node->name = NULL;
// 	new_node->next = NULL;
// 	new_node->value = NULL;
// }



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

char	**ft_unset(t_data *data, int	save)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	while (data->envp[i])
		i++;
	new_env = malloc(sizeof(char *) * i);
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
	i = 0;
	while (data->envp[i])
		free(data->envp[i++]);
	free(data->envp);
	// printf("nouvelle env ;\n");
	// show_tab(new_env);
	return (new_env);
}


char	*ft_joinunset(char *str)
{
	int		i;
	int		j;
	int		len;
	char	*join;

	len = ft_strlen(str);
	i = 0;
	j = 0;
	join = malloc(sizeof(char) * (len + 2));
	if (!join)
		return (NULL);
	while (str[i])
	{
		join[j] = str[i];	
		j++;
		i++;
	}
	join[j] = '=';
	j++;
	join[j] = '\0';
	return (join);
}

void	check_unset(t_data *data, char	*str)
{
	int		i;
	char	*join_eg;
	
	i = 0;
	join_eg = ft_joinunset(str);
	// printf("joineg = {%s}\n", join_eg);
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], join_eg, ft_strlen(join_eg)) == 0)
		{
			// printf("FOUNDED");
			data->envp = ft_unset(data, i);
			data->exit_status = 0;
			break;
		}
		i++;
	}
	free(join_eg);
}

void	ft_pwd(t_data *data)
{
	char path[1024];

    if (getcwd(path, sizeof(path)) != NULL)
	{
		data->exit_status = 0;
        printf("%s\n", path);
	}
    else
	{
		data->exit_status = 1;
        perror("getcwd");
	}
}


int	check_builtin(t_data *data)
{
	if (ft_strlcmp(data->list->content, "pwd") == 0 ||ft_strlcmp(data->list->content, "\"pwd\"") == 0)
	{
		printf("ici dans pwd built_in\n");
		if (get_nbr_node(data->list) > 1)
			return (printf("pwd: too many arguments\n"), 1);
		ft_pwd(data);
		return (1);
	}
	if (ft_strlcmp(data->list->content, "unset") == 0 ||ft_strlcmp(data->list->content, "\"unset\"") == 0)
	{
		// printf("in unset!\n");
		data->list = data->list->next;
		check_unset(data, data->list->content);	
		// ft_unset(*data);
		return (1);
	}
	if (ft_strlcmp(data->list->content, "exit") == 0 ||ft_strlcmp(data->list->content, "\"exit\"") == 0)
	{
		ft_exit(data);
		return (1);
	}
	if (ft_strlcmp(data->list->content, "echo") == 0 ||ft_strlcmp(data->list->content, "\"echo\"") == 0)
	{
		ft_echo(*data);
		return (1);
	}
	if (ft_strlcmp(data->list->content, "cd") == 0 ||ft_strlcmp(data->list->content, "\"cd\"") == 0)
	{
		ft_cd(data);
		return (1);
	}
	if (ft_strlcmp(data->args[0], "env") == 0 ||ft_strlcmp(data->args[0], "\"env\"") == 0)
	{
		//if(!data.env_var)
		//	fill_env(&data);
		show_tab(data->envp);
		//data->exit_status = 0;
		return (1);
	}
	if (ft_strlcmp(data->args[0], "export") == 0 ||ft_strlcmp(data->args[0], "\"export\"") == 0)
	{
		printf("in export!\n");
		printf("nombre de noeud : %d\n", get_nbr_node(data->list));
		if (get_nbr_node(data->list) == 1)
		{
			show_tab(data->envp);
		}
		data->list = data->list->next;
		while(data->list != NULL)
		{
			// printf("{%s}\n", data->list->content);
			if (check_export_compatibility(data->list->content) == 0)
			{
				printf("Good format!\n");
				data->envp = add_export(data, data->list->content);
			}
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