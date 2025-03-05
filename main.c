/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/03/05 17:40:31 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
	{
		printf("tab existe pas!\n");
		return;
	}
	while (tab[i])
	{
		printf("ligne %d :%s\n", i, tab[i]);
		i++;
	}
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
        /* Verifier les quotes non ferme uniquement si l'entre ne contient pas de heredoc ("<<").
		 Si l'entre contient un heredoc, on suppose que le contenu sera traite separement.*/
        if (!ft_strstr(data.input, "<<") && (count_global_quotes(data.input) % 2 != 0))
        {
            data.exit_status = 127;
            printf("minishell: unclosed quote detected\n");
            free(data.input);
            continue; // Retour au prompt
        }
        // Traitement de l'entre
        if (ft_strchr(data.input, '|'))
			execute_pipex(&data);
        else
        {
            check_redirect(&data);
            data.args = ft_split(data.input, ' ');
			printf("tab apres split\n");
			show_tab(data.args);
            cut_empty(data.args, &data);
            data.list = add_chained_list(&data);
            if (check_dollar(&data) != 0)
                data.args = list_to_args(&data);
            data.args = skip_quotes(&data);
            data.list = add_chained_list(&data);
            //check_exit_status(&data);
			//printf("list apres le check exit\n");
			//show_list(data.list);
            if (case_redirection(&data) == 1)
                parsing(data.envp, &data);
        }
        free(data.input);
    }
    return (0);
}
