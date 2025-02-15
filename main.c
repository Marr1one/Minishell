/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/02/15 17:28:04 by maissat          ###   ########.fr       */
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

int	check_unclosed(t_data *data)
{
	t_token	*list;
	int		len;
	
	list = data->list;
	while (list)
	{
		len = ft_strlen(data->list->content);
		if (data->list->content[0] == '"' && data->list->content[len - 1] != '"')
			return (1);
		list = list->next;
	}
	return (0);
}

void	check_exit_status(t_data *data)
{
	t_token	*list;

	list = data->list;
	while (list)
	{
		if (ft_strlcmp(list->content, "$?") == 0)
		{
			//printf("$? trouvE!\n");
			//printf("before : list content = {%s}\n", list->content);
			printf("data.exit_status = {%d}\n", data->exit_status);
			list->content = ft_strdup(ft_itoa(data->exit_status));
			printf("after ; list.content = {%s}\n", list->content);
		}
		list = list->next;
	}
}

int	return_exit_status(t_data *data)
{
	t_token	*list;

	list = data->list;
	while (list)
	{
		if (ft_strlcmp(list->content, "$?") == 0)
		{
			//printf("$? trouvE!\n");
			//printf("before : list content = {%s}\n", list->content);
			//printf("data.exit_status = {%d}\n", data->exit_status);
			list->content = ft_strdup(ft_itoa(data->exit_status));
			//printf("after ; list.content = {%s}\n", list->content);
			return (1);
		}
		list = list->next;
	}
	return (0);
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
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		data.input = readline("\033[0;32mminishell$\033[0m ");
		if (!data.input)
			break ;
		check_redirect(&data);
		// printf("input = {%s}\n", data.input);
		data.args = ft_split(data.input, ' ');
		
		cut_empty(data.args, &data);
		
		//printf("tab juste apres le split de input :\n");
		//show_tab(data.args);
			//show_tab(data.args);
		data.list = add_chained_list(&data);
		//printf("list juste apres sa creation :\n");
		//show_list(data.list);
		check_exit_status(&data);
		check_dollar(&data);
		//show_list(data.list);
		if (case_redirection(&data, data.envp) == 1)
		{
			// printf("on  va rentrer dans parsing car on trouve pas >\n");
			parsing(data.input, data.envp, &data);
		}
		add_history(data.input);						
	}
	return (0);
}
