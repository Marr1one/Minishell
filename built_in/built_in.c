/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 13:19:59 by maissat           #+#    #+#             */
/*   Updated: 2025/03/22 19:35:08 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void ft_cd(t_data *data)
// {
// 	char	*old_pwd;
// 	char	current_pwd[1024];

// 	if (getcwd(current_pwd, sizeof(current_pwd)) == NULL)
// 	{
// 		perror("getcwd");
// 	}
// 	old_pwd = getenv("OLDPWD");
// 	//printf("old_pwd = %s\n", old_pwd);
// 	if (data->args[1] && ft_strlcmp(data->args[1], "-") == 0)
// 	{
// 		if (!old_pwd)
// 		{
// 			printf("minishell: cd: OLDPWD not set\n");
// 			data->exit_status = 1;
// 			return ;
// 		}
// 		printf("%s\n", old_pwd);
// 		if (chdir(old_pwd) != 0)
// 			perror("cd");
// 		data->exit_status = 0;
// 		return;
// 	}
// 	else if (data->args[1] && chdir(data->args[1]) != 0)
// 	{
// 		data->exit_status = 1;
// 		perror("cd");
// 	}
// 	else
// 		data->exit_status = 0;
// }

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




// char	*ft_strdupenv(char	*str, int start, int	i)
// {
// 	int		k;
// 	char	*dup;

// 	k = 0;
// 	//printf("str[start] = %c, et str[i] (fin) = %c\n", str[start], str[i]);
// 	dup = ft_malloc(sizeof(char) * (i - start) + 1);
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
// 	new_node = ft_malloc(sizeof(t_env));
// 	if (!new_node)
// 		return;
// 	new_node->name = NULL;
// 	new_node->next = NULL;
// 	new_node->value = NULL;
// }

// char	*take_before(char	*str)
// {
// 	int	i;
// 	char	*new_str;

// 	i = 0;
// 	while (str[i] && str[i] != '=')
// 		i++;
// 	new_str = ft_malloc(sizeof(char) * i + 1);
// 	i = 0;
// 	while (str[i] && str[i] != '=')
// 	{
// 		new_str[i] = str[i];
// 		i++;
// 	}
// 	new_str[i] = '\0';
// 	return (new_str);
// }

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

char	**ft_unset(t_data *data, int	save)
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
	i = 0;
	// while (data->envp[i])
		// free(data->envp[i++]);
	// free(data->envp);
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
	join = ft_malloc(sizeof(char) * (len + 2));
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
	// free(join_eg);
}

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	show_tab_export(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		printf("export %s\n", tab[i]);
		i++;
	}
}

//la on verifie le premier argument, si cest parmis un builtin on lance la fonction adequates.
int	check_builtin(t_data *data)
{
	(void)data;
// 	// if (data->list && (ft_strlcmp(data->list->content, "pwd") == 0 ||ft_strlcmp(data->list->content, "\"pwd\"") == 0))
// 	// {
// 	// 	//printf("dans mon pwd a moi\n");
// 	// 	destroy_node_quotes(data); //detruit les noeuds ou ya juste des guillemets ; pwd "" devient pwd.
// 	// 	// ft_pwd(data);
// 	// 	return (1);
// 	// }
// 	// if (data->list && (ft_strlcmp(data->list->content, "unset") == 0 ||ft_strlcmp(data->list->content, "\"unset\"") == 0))
// 	// {
// 	// 	data->list = data->list->next;
// 	// 	check_unset(data, data->list->content);	
// 	// 	return (1);
// 	// }
// 	// if (data->list && (ft_strlcmp(data->list->content, "exit") == 0 ||ft_strlcmp(data->list->content, "\"exit\"") == 0))
// 	// {
// 	// 	ft_exit(data);
// 	// 	return (1);
// 	// }
// 	// if (data->list && (ft_strlcmp(data->list->content, "echo") == 0 ||ft_strlcmp(data->list->content, "\"echo\"") == 0))
// 	// {
// 	// 	ft_echo(*data);
// 	// 	return (1);
// 	// }
// 	// if (data->list && (ft_strlcmp(data->list->content, "cd") == 0 ||ft_strlcmp(data->list->content, "\"cd\"") == 0))
// 	// {
// 	// 	ft_cd(data);
// 	// 	return (1);
// 	// }
// 	// if (data->list && (ft_strlcmp(data->args[0], "env") == 0 ||ft_strlcmp(data->args[0], "\"env\"") == 0))
// 	// {
// 	// 	show_tab(data->envp);
// 	// 	return (1);
// 	// }
// 	//if (data->list && (ft_strlcmp(data->args[0], "export") == 0 ||ft_strlcmp(data->args[0], "\"export\"") == 0))
// 	//{
// 	//	export(data);
// 	//	return (1);
// 	//}
	return (0);
}
