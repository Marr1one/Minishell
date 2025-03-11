/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 18:50:04 by maissat           #+#    #+#             */
/*   Updated: 2025/03/11 17:31:17 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



//int main(int argc, char **argv, char **envp)
//{
//    t_data data;
//    (void)argv;
    
//    if (argc != 1)
//        return (printf("Usage : ./minishell\n"));
    
//    ft_memset(&data, 0, sizeof(t_data));
//    data.gc = *get_gc();
//    data.envp = copy_env(envp);
//    signal(SIGINT, sigint_handler);
//    signal(SIGQUIT, SIG_IGN);

//    while (1)
//    {
//        data.input = readline("\033[0;32mminishell$\033[0m ");
//        if (!data.input)
//            break;
//        add_history(data.input); 
//        /* Verifier les quotes non ferme uniquement si l'entre ne contient pas de heredoc ("<<").
//		 Si l'entre contient un heredoc, on suppose que le contenu sera traite separement.*/
//        if (!ft_strstr(data.input, "<<") && (count_global_quotes(data.input) % 2 != 0))
//        {
//            data.exit_status = 127;
//            printf("minishell: unclosed quote detected\n");
//            free(data.input);
//            continue; // Retour au prompt
//        }
//        if (ft_strchr(data.input, '|'))
//			execute_pipex(&data);
//        else
//        {
//            check_redirect(&data);
//            data.args = custom_split(data.input, ' ');
//            cut_empty(data.args, &data);
        //data.list = add_chained_list(&data);
//            if (check_dollar(&data) != 0)
//                data.args = list_to_args(&data);
//            data.args = skip_quotes(&data);
//            data.list = add_chained_list(&data);
//			destroy_empty_node(&data);
//            if (case_redirection(&data) == 1)
//                parsing(data.envp, &data);
//        }
//        free(data.input);
//    }
//    return (0);
//}


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
