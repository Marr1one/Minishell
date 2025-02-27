/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/02/27 19:24:20 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return;
	while (tab[i])
	{
		printf("%s\n", tab[i]);
		i++;
	}
}

char	**cut_last(char **tab, int	i)
{
	char	**new_tab;
	int		j;
	
	j = 0;
	new_tab = ft_malloc(sizeof(char *) * (i + 1));
	if (!new_tab)
		return (NULL);
	while (j < i)
	{
		new_tab[j] = tab[j];
		j++;
	}
	new_tab[j] = NULL;
	return (new_tab);
}

void	cut_empty(char **tab, t_data *data)
{
	int		i;
	
	i = 0;
	while (tab[i])
	{
		if (tab[i + 1] == NULL)
		{
			if (tab[i][0] == '\0')
				data->args = cut_last(tab, i);
		}
		i++;
	}
}

void	show_list(t_token *list)
{
	while(list)
	{
		printf("noeud : {%s}\n", list->content);
		list = list->next;
	}
}

void	show_malloc_list(t_malloc *list)
{
	int	i;
	t_malloc_node	*current;

	i = 0;
	if (!list || !list->first)
	{
		printf("rien a afficher\n");
		return;	
	}
	current = list->first;
	while(current)
	{
		printf("noeud  %d : {%p}\n", i,current->addr);
		current = current->next;
		i++;
	}
}

//int	check_unclosed(t_data *data)
//{
//	t_token	*list;
//	int		len;
	
//	list = data->list;
//	while (list)
//	{
//		//len = ft_strlen(data->list->content);
//		//if (data->list->content[0] == '"' && data->list->content[len - 1] != '"')
//		//	return (1);
//		list = list->next;
//	}
//	return (0);
//}





//void	destroy_quotes(t_data *data)
//{
//	int		i;
//	int		j;
//	int		k;
//	char 	**args;
	
//	args = data->args;
//	i = 0;
//	k = 0;
//	while (data->args[i])
//	{
//		j = 0;
//		while (data->args[i][j] == '"')
//			j++;
//		if (data->args[i][j] == '\0')
//		{
//			free(data->args[i]);
//		}
//		else
//		{
//			data->args[k] = data->args[i];
//			k++;
//		}
//		i++;
//	}
//	data->args[k] = NULL;
//}

int	list_len(t_token *list)
{
	int	i;
	t_token *current;
	
	i = 0;
	current = list;
	while (current)
	{
		current = current->next;
		i++;
	}
	return (i);
}

char	**list_to_args(t_data *data)
{
	t_token *current;
	char	**new_args;
	int		i;
	
	i = 0;
	new_args = ft_malloc(sizeof(char *) * (list_len(data->list) + 1));
	if (!new_args)
		return (NULL);
	current = data->list;
	while (current)
	{
		new_args[i] = ft_strdup(current->content);
		i++;
		current = current->next;
	}
	new_args[i] = NULL;
	return (new_args);
}

void	update_list(t_data *data)
{
	t_token	*list;
	t_token	*temp;
	list = data->list;

	while (list)
	{
		while (list && list->content[0] == '$')
		{
			temp = list;
			
		}
			
		list = list->next;
	}
}

int main(int argc, char **argv, char **envp)
{
    t_data data;
	(void)argv;
	
    if (argc != 1)
        return (printf("Usage : ./minishell\n"));
    ft_memset(&data, 0, sizeof(t_data));
    data.gc = *get_gc();
    data.envp = copy_env(envp);
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);

    while (1)
    {
        data.input = readline("\033[0;32mminishell$\033[0m ");
        if (!data.input)
            break;
        add_history(data.input);
		if (count_global_quotes(data.input) % 2 != 0) // permet de gerer le cas de quotes non fermees
		{
			data.exit_status = 127;
			printf("minishell: unclosed quote detected\n");
		}
        else
		{
			if (ft_strchr(data.input, '|'))
			{
				printf("on trouve |");
				execute_pipex(&data);
			}
			else
			{
				check_redirect(&data);
				data.args = ft_split(data.input, ' ');
				cut_empty(data.args, &data);
				data.list = add_chained_list(&data);
				check_dollar(&data);
				data.args = list_to_args(&data);
				data.args = skip_quotes(&data);
				data.list = add_chained_list(&data);
				check_exit_status(&data);
				printf("list juste avant le parsing\n");
				show_list(data.list);
				if (case_redirection(&data, data.envp) == 1)
					parsing(data.input, data.envp, &data);
			}
		}
        free(data.input);
    }
			
    return (0);
}
