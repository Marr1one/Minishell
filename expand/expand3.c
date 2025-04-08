/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:12:20 by braugust          #+#    #+#             */
/*   Updated: 2025/04/08 16:42:34 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_dollar_len(const char *arg, int *i, t_data *data)
{
	int	len;

	len = 0;
	if (!arg[*i])
		return (1);
	if (arg[*i] == '?')
	{
		len = len_exit_status(data);
		(*i)++;
	}
	else
		len = len_var_value(arg, i, data);
	printf("DEBUG: handle_dollar_len, len = %d at idx.i = %d\n", len, *i);
	return (len);
}

int	calc_final_len(const char *arg, t_data *data)
{
	int	i;
	int	final_len;

	i = 0;
	final_len = 0;
	data->in_quote = 0;
	while (arg[i])
	{
		if (handle_quotes(arg[i], data))
		{
			final_len++;
			i++;
			continue ;
		}
		if (arg[i] == '$' && data->in_quote != 1)
			final_len += handle_dollar_len(arg, &i, data);
		else
			final_len++;
		i++;
	}
	printf("DEBUG: Calcul final_len = %d\n", final_len);
	return (final_len);
}

int	check_variable_in_env(char *var_name, t_data *data)
{
	char	*value;

	value = check_env(data, var_name);
	if (value == NULL)
		return (0);
	return (1);
}

// Ajoute la valeur développée d'une variable dans la chaîne de destination
int	append_var_value(char *result, int *j, char *var_name, t_data *data)
{
	char	*var_value;
	char	*exit_str;
	int		k;

	exit_str = NULL;
	if (ft_strcmp(var_name, "?") == 0)
	{
		exit_str = ft_itoa(data->exit_status);
		k = 0;
		while (exit_str[k])
			result[(*j)++] = exit_str[k++];
		printf("DEBUG: append_var_value, added exit status: %s\n", exit_str);
		return (0);
	}
	else
	{
		if (check_variable_in_env(var_name, data) == 0)
			return (1);
		var_value = check_env(data, var_name);
		if (!var_value)
			var_value = "";
		k = 0;
		while (var_value[k])
			result[(*j)++] = var_value[k++];
		printf("DEBUG: append_var_value, added variable: %s = %s\n", var_name, var_value);
		return (0);
	}
}

// Gère l'expansion des signes dollar en interprétant les
// variables d'environnement ou spéciales
int	handle_dollar(char *result, char *arg, t_idx *idx, t_data *data)
{
	int		dollar_count;
	char	*var_name;
	int		k;
	int		error;

	dollar_count = get_dollar_count(arg, &idx->i);
	k = dollar_count / 2;
	while (k-- > 0)
		result[idx->j++] = '$';
	if (dollar_count % 2 != 0)
	{
		var_name = extract_var_name(arg, &idx->i);
		if (var_name)
		{
			error = append_var_value(result, &idx->j, var_name, data);
			if (error)
				return (1);
		}
		else
			result[idx->j++] = '$';
	}
	return (0);
}
