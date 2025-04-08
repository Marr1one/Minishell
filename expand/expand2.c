/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:11:17 by braugust          #+#    #+#             */
/*   Updated: 2025/04/08 16:50:13 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_redirect(char c)
{
	if (c == '>' || c == '<')
		return (1);
	return (0);
}

int	is_pipe(char c)
{
	if (c == '|')
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

int	len_exit_status(t_data *data)
{
	char	*exit_str;
	int		len;

	exit_str = ft_itoa(data->exit_status);
	if (!exit_str)
		return (0);
	len = ft_strlen(exit_str);
	return (len);
}

int	len_var_value(const char *arg, int *i, t_data *data)
{
	char	*var_name;
	char	*var_value;
	int		len;

	var_name = extract_var_name(arg, i);
	if (!var_name)
		return (0);
	var_value = check_env(data, var_name);
	if (!var_value)
		var_value = "";
	len = ft_strlen(var_value);
	return (len);
}
