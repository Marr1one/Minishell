/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/02/22 16:37:58 by maissat          ###   ########.fr       */
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
		printf("noeud : {%s}\n", list->content);
		list = list->next;
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

int	len_without_quotes(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] != '"')
			count++;
		i++;
	}
	return (count);
}

char	**skip_quotes(t_data *data)
{	
	char 	**new_args;
	int		i;
	int		j;
	int		len;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	while (data->args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (data->args[i])
	{
		j = 0;
		k = 0;
		len = len_without_quotes(data->args[i]);
		new_args[i] = malloc(sizeof(char) * (len + 1));
		while (data->args[i][j])
		{
			if(data->args[i][j] == '"' || data->args[i][j] == '\'')
				j++;
			else
			{
				new_args[i][k] = data->args[i][j];
				k++;
				j++;
			}
		}
		new_args[i][k] = '\0';
		i++;
	}
	new_args[i] = NULL;
	free(data->args);
	return (new_args);
}

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
