/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_show.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 02:11:10 by maissat           #+#    #+#             */
/*   Updated: 2025/04/09 16:14:28 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

const char	*get_token_type_name(t_type type)
{
	if (type == CMD)
		return ("CMD");
	if (type == ARG)
		return ("ARG");
	if (type == INFILE)
		return ("INFILE");
	if (type == OUTFILE_APPEND)
		return ("OUTFILE_APPEND");
	if (type == OUTFILE_TRUNC)
		return ("OUTFILE_TRUNC");
	if (type == HEREDOC)
		return ("HEREDOC");
	if (type == PIPE)
		return ("PIPE");
	if (type == FICHIER)
		return ("FICHIER");
	return ("UNKNOWN");
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
	while (list)
	{
		printf("noeud %d : {%s}, type = {%s}\n", i, list->content,
			get_token_type_name(list->type));
		list = list->next;
		i++;
	}
}

void	show_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		printf("%s\n", tab[i++]);
}
