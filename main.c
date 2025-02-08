/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/02/08 16:25:29 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_tab(char **tab)
{
	int	i;

	i = 0;
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
	new_tab = malloc(sizeof(char *) * (i + 1));
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
		printf("noeud %d : {%s}\n", list->index, list->content);
		list = list->next;
	}
}

int	main(int argc, char **argv , char **envp)
{
	
	(void)argv;
	t_data data;

	if (argc != 1)
		return (printf("Usage : ./minishell\n"));
	ft_memset(&data, 0, sizeof(t_data));
	data.envp = copy_env(envp);
	signal(SIGINT, sigint_handler);
	while (1)
	{
		data.input = readline("\033[0;32mminishell$\033[0m ");
		if (!data.input)
			break ;
		data.args = ft_split(data.input, ' ');
		cut_empty(data.args, &data);
		//show_tab(data.args);
			//show_tab(data.args);
		data.list = add_chained_list(&data);
		check_dollar(&data);
		show_list(data.list);
		if (case_redirection(&data, data.envp) == 1)
		{
			// printf("on  va rentrer dans parsing car on trouve pas >\n");
			parsing(data.input, data.envp, &data);
		}
		add_history(data.input);						
	}
	return (0);
}
