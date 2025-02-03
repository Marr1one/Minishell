/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 19:39:51 by maissat           #+#    #+#             */
/*   Updated: 2025/02/03 19:56:48 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_alpha(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c<= 'Z'))
		return (0);
	else
		return (1);
}

int	check_env_var(t_data *data)
{
	char	*str;
	int		i;
	
	i = 0;
	str = data->input;
	while (is_alpha(str[i]) == 0)
		i++;
	if (str[i] == '=') //ici il faut juste que jajoute ce quil ya dans jusqua i dans une case ; NAME
		i++;
	else 
		return (1);
	while (is_alpha(str[i]) == 0)
		i++;
	if (str[i] == '\0') //ici il faut juste que jajoute ce quil ya entre = et \0 dans une varaible VALUE
		return (0);
	else 
		return (1);
	
}