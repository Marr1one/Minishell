/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 18:50:04 by maissat           #+#    #+#             */
/*   Updated: 2025/02/07 18:50:44 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// void	fill_env(t_data *data)
// {
// 	int	i;
// 	t_env *new_node;
// 	t_env *list;

// 	list = data->env_var;
// 	i = 0;
// 	while(data->envp[i])
// 	{
// 		new_node = malloc(sizeof(t_env));
// 		if (!new_node)
// 			return ;
// 		new_node->name = get_name(data->envp[i]);
// 		new_node->value = get_value(data->envp[i]);
// 		new_node->next = NULL;
// 		if (!list)
// 			list = new_node;
// 		else
// 			find_last_node(list)->next = new_node;
// 		i++;
// 	}
// 	data->env_var = list;
// 	//show_list(data->env_var);
// }


// char	*get_name(char	*str)
// {
// 	int	i;
// 	char *name;

// 	i = 0;
// 	while(str[i] !=  '=')
// 		i++;
// 	name = ft_strdupenv(str, 0, i);
// 	return (name);
// }

// char	*get_value(char	*str)
// {
// 	int		i;
// 	int		save;
// 	char	*value;

// 	i = 0;
// 	while (str[i] != '=')
// 		i++;
// 	i ++;
// 	save = i;
// 	while(str[i] !=  '\0')
// 		i++;
// 	value = ft_strdupenv(str, save, i);
// 	return (value);
// }


// t_env	*find_last_node(t_env	*list)
// {
// 	if (!list)
// 		return (NULL);
// 	while (list->next != NULL)
// 		list = list->next;	
// 	return (list);
// }

//void	show_list(t_env *list)
//{
//	while (list)
//	{
//		printf("%s=%s\n", list->name, list->value);
//		list = list->next;
//	}
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
