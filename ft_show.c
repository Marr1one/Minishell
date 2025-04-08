/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_show.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 02:11:10 by maissat           #+#    #+#             */
/*   Updated: 2025/04/08 04:21:26 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	show_files(t_file *list_file)
{
	t_file *current;
	int		i;

	current = list_file;
	i = 0;
	while (current)
	{
		printf("file %d : mode = %s, path = %s\n", i + 1, get_token_type_name(current->mode), current->path);
		current = current->next;
		i++;
	}
}

void	show_list_cmd(t_cmd *list)
{
	t_cmd *current;
	int		i;

	current = list;
	i = 0;
	while (current)
	{
		printf("commande %d\n", i +1);
		i++;
		printf("args\n");
		show_tab(current->args);
		printf("files\n");
		show_files(current->files);
		current = current->next;
	}
}

int	char_in_string(char *str, char c)
{
	int	i;
	
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	show_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (1);
	while (env[i])
	{
		if (char_in_string(env[i], '=') == 1)
			printf("%s\n", env[i]);
		i++;
	}
	return (0);
}

void	show_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
	{
		printf("tab existe pas!\n");
		return;
	}
	if (!*tab)
	{
		printf("il ya rien dans tab!\n");
		return;
	}
	while (tab[i])
	{
		printf("{%s}\n", tab[i]);
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

const char	*get_token_type_name(t_type type)
{
	if (type == CMD)
		return "CMD";
	if (type == ARG)
		return "ARG";
	if (type == INFILE)
		return "INFILE";
	if (type == OUTFILE_APPEND)
		return "OUTFILE_APPEND";
	if (type == OUTFILE_TRUNC)
		return "OUTFILE_TRUNC";
	if (type == HEREDOC)
		return "HEREDOC";
	if (type == PIPE)
		return "PIPE";
	if (type == FICHIER)
		return "FICHIER";
	return "UNKNOWN";
}

void	show_list(t_token *list)
{
	int	i;

	i = 0;
	if (!list)
	{
		printf("list meme pas alloue\n");
		return ;
		
	}
	while(list)
	{
		printf("noeud %d : {%s}, type = {%s}\n", i, list->content, get_token_type_name(list->type));
		list = list->next;
		i++;
	}
}

