/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 19:31:51 by maissat           #+#    #+#             */
/*   Updated: 2025/03/08 16:08:09 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_export_compatibility(char *str)
{
	int	i;
	int	quote;
	
	i = 0;
	quote = 0;
	while (is_alpha(str[i]) == 0)
		i++;
	if (str[i] == '=')
		i++;
	else
		return (1);
	if (str[i] == '"')
		quote = 1;
	i++;
	while (is_alpha(str[i]) == 0 || str[i] == '/')
		i++;
	if (str[i] == '\0')
		return (0);
	else if ((quote == 1 && str[i] == '"'))
		return (0);
	else
		return (1);
}