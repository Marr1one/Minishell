/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:13:09 by braugust          #+#    #+#             */
/*   Updated: 2025/04/08 20:07:28 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Construit la chaîne finale après les expansions à partir
// de la chaîne d'entrée
int	build_final_string(char *result, char *arg, t_data *data, int final_len)
{
	t_idx	idx;
	int		error;

	idx.i = 0;
	idx.j = 0;
	data->in_quote = 0;
	while (arg && arg[idx.i])
	{
		if (handle_quotes(arg[idx.i], data))
			result[idx.j++] = arg[idx.i++];
		else if (arg[idx.i] == '$' && data->in_quote != 2)
		{
			error = handle_dollar(result, arg, &idx, data);
			if (error)
				return (1);
		}
		else
		{
			if (idx.j >= final_len)
				return (1);
			result[idx.j++] = arg[idx.i++];
		}
	}
	printf("dans i %d\n", idx.i);
	printf("dans j %d\n", idx.j);
	result[idx.j] = '\0';
	return (0);
}

// Compte les signes dollar consécutifs à partir d'une position donnée
int	get_dollar_count(const char *arg, int *i)
{
	int	count;

	count = 0;
	while (arg[*i] == '$')
	{
		count++;
		(*i)++;
	}
	return (count);
}

static char	*handle_special_var(int *i)
{
	char	*var_name;

	(*i)++;
	var_name = ft_malloc(2);
	if (!var_name)
		return (NULL);
	var_name[0] = '?';
	var_name[1] = '\0';
	return (var_name);
}

// Extrait un nom de variable à partir d'une position donnée dans une chaîne
char	*extract_var_name(const char *arg, int *i)
{
	int		start;
	int		len;
	char	*var_name;
	int		k;

	if (arg[*i] == '?')
		return (handle_special_var(i));
	start = *i;
	while (arg[*i] && (isalnum((unsigned char)arg[*i]) || arg[*i] == '_'))
		(*i)++;
	len = *i - start;
	if (len == 0)
		return (NULL);
	var_name = ft_malloc(len + 1);
	if (!var_name)
		return (NULL);
	k = 0;
	while (k < len)
	{
		var_name[k] = arg[start + k];
		k++;
	}
	var_name[len] = '\0';
	return (var_name);
}

// Mise en place grace a toute les fonctions
char	*expand_string(char *arg, t_data *data)
{
	char	*result;
	int		final_len;
	int		error;

	final_len = calc_final_len(arg, data); // LE PROBLEME VIENT DE LA PAS ASSEX DE PLACE
	result = ft_malloc(final_len + 1000);
	if (!result)
		return (NULL);
	error = build_final_string(result, arg, data, final_len);
	if (error)
		return (NULL);
	return (result);
}
