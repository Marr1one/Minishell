/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:27:11 by maissat           #+#    #+#             */
/*   Updated: 2025/04/03 19:38:02 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_substr_qte(char *str, int start, int len)
{
	int		i;
	char	*new_str;

	i = 0;
	if (!str)
		return (NULL);
	new_str = ft_malloc(sizeof(char) * (len + 1));
	if (new_str == NULL)
		return (NULL);
	while (str[start] && i < len)
		new_str[i++] = str[start++];
	new_str[i] = '\0';
	return (new_str);
}

char	*ft_substr(char *str, int start, int end)
{
	int		i;
	char	*new_str;

	i = 0;
	if (!str || start >= end)
		return (NULL);
	new_str = ft_malloc(sizeof(char) * (end - start + 1));
	if (new_str == NULL)
		return (NULL);
	while (str[start] && start < end)
		new_str[i++] = str[start++];
	new_str[i] = '\0';
	return (new_str);
}

void	add_arg(t_token *curr_tkn, t_cmd *curr_cmd, int	*i)
{
	if (curr_tkn->type == CMD || curr_tkn->type == ARG)
		curr_cmd->args[(*i)++] = curr_tkn->content;
	else if (curr_tkn->type == PIPE)
	{
		curr_cmd->args[*i] = NULL;
		i = 0;
		curr_cmd = curr_cmd->next;
		curr_cmd->args = ft_malloc(sizeof(char *) * \
			(count_arguments(curr_tkn) + 1));
	}
}
