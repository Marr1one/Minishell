/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/02/25 17:04:24 by maissat          ###   ########.fr       */
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

int	main(int argc, char **argv , char **envp)
{
	
	(void)argv;
	t_data data;

	if (argc != 1)
		return (printf("Usage : ./minishell\n"));
	ft_memset(&data, 0, sizeof(t_data));
	data.gc = *get_gc();
	data.envp = copy_env(envp);
	signal(SIGINT, sigint_handler); // gestion de control+c
	signal(SIGQUIT, SIG_IGN);// gestion de ctrl + '\'
	while (1)
	{
		data.input = readline("\033[0;32mminishell$\033[0m ");
		if (!data.input)
			break ; //gestion de ctrl +d
		check_redirect(&data); //reecrit bien les redirection, ls>out devient ls > out, ajoute des espaces entre les >
		data.args = ft_split(data.input, ' ');
		cut_empty(data.args, &data); //dans le cas ou on fait ls avec un espace apres, fait fonctionner ls, sans ca 
		// ls espace ne fonctionne pas
		data.args = skip_quotes(&data); //refait un nouveau tableau dargument en skippant les quotes ; {"ls", "} devient
		// {ls, }.
		data.list = add_chained_list(&data); //creer une liste chainee correpondant au tableau dargs
		check_exit_status(&data); // si un argument est '$?', le remplace par lexit status.
		check_dollar(&data); //verifie si ya un $, si yen a un, on parcourt lenv, si on trouve que ca correspond
		// a une ligne, on met sa valeur dans content.
		if (case_redirection(&data, data.envp) == 1)//explication dans la fonction
		{
			parsing(data.input, data.envp, &data);
		}
		add_history(data.input);//permet que lhistorique marche quand on fait fleche du haut 						
	}
	return (0);
}
