/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:30:52 by maissat           #+#    #+#             */
/*   Updated: 2025/03/31 16:38:11 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	len_exit_status(t_data *data)
{
	char	*exit_str;
	int		len;

	exit_str = ft_itoa(data->exit_status);
	if (!exit_str)
		return (0);
	len = ft_strlen(exit_str);
	free(exit_str);
	return (len);
}

static int	len_var_value(const char *arg, int *i)
{
	char	*var_name;
	char	*var_value;
	int		len;

	var_name = extract_var_name(arg, i);
	if (!var_name)
		return (1);
	var_value = getenv(var_name);
	if (!var_value)
		var_value = "";
	len = ft_strlen(var_value);
	free(var_name);
	return (len);
}

static int	handle_dollar_len(const char *arg, int *i, t_data *data)
{
	int	len;

	len = 0;
	if (arg[*i] == '?')
	{
		len = len_exit_status(data);
		(*i)++;
	}
	else
		len = len_var_value(arg, i);
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
		if (arg[i] == '$' && data->in_quote != 2)
		{
			i++;
			final_len += handle_dollar_len(arg, &i, data);
		}
		else
		{
			final_len++;
			i++;
		}
	}
	return (final_len);
}

int check_variable_in_env(const char *var_name)
{
    char *value;

    value = getenv(var_name);
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

	if (ft_strcmp(var_name, "?") == 0)
	{
		exit_str = ft_itoa(data->exit_status);
		k = 0;
		while (exit_str[k])
			result[(*j)++] = exit_str[k++];
		free(exit_str);
		return(0);
	}
	else
	{
		if (check_variable_in_env(var_name) == 0)
			return (1);
		var_value = getenv(var_name);
		if (!var_value)
			var_value = "";
		k = 0;
		while (var_value[k])
			result[(*j)++] = var_value[k++];
		return (0);
	}
}
// Gère l'expansion des signes dollar en interprétant les variables d'environnement ou spéciales
int handle_dollar(char *result, const char *arg, t_idx *idx, t_data *data)
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
    }
	return (0);
}

// Construit la chaîne finale après les expansions à partir de la chaîne d'entrée
int	build_final_string(char *result, const char *arg, t_data *data)
{
    t_idx idx;
    int error;

    idx.i = 0;
    idx.j = 0;
    data->in_quote = 0;
    while (arg[idx.i])
    {
        if (handle_quotes(arg[idx.i], data))
            result[idx.j++] = arg[idx.i++];
        else if (arg[idx.i] == '$' && data->in_quote != 2)
		{
            error = handle_dollar(result, arg, &idx, data);
			if (error)
			{
				return (1);
			}
		}
        else
            result[idx.j++] = arg[idx.i++];
    }
    result[idx.j] = '\0';
	return (0);
}

// Compte les signes dollar consécutifs à partir d'une position donnée
int	get_dollar_count(const char *arg, int *i)
{
	int count;

	count = 0;
	while (arg[*i] == '$')
	{
		count++;
		(*i)++;
	}
	return (count);
}

// Extrait un nom de variable à partir d'une position donnée dans une chaîne
char	*extract_var_name(const char *arg, int *i)
{
	int		start;
	int		len;
	char	*var_name;
	int		k;

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
char	*expand_string(const char *arg, t_data *data)
{
	char	*result;
	int	final_len;
	int	error;

	
	final_len = calc_final_len(arg, data);
	result = ft_malloc(final_len + 1);
	if (!result)
		return (NULL);
	error = build_final_string(result, arg, data);
	if (error)
	{
		return (NULL);
	}
	return (result);
}

void	expand_all(t_cmd *cmd, t_data *data)
{
	t_cmd	*current_cmd;
	int		i;
	char	*expanded;
	t_file	*current_file;

	current_cmd = cmd;
	while (current_cmd)
	{
		i = -1;
		while (current_cmd->args && current_cmd->args[++i])
		{
			expanded = expand_string(current_cmd->args[i], data);
			free(current_cmd->args[i]);
			current_cmd->args[i] = expanded;
		}
		current_file = current_cmd->files;
		while (current_file)
		{
			expanded = expand_string(current_file->path, data);
			free(current_file->path);
			current_file->path = expanded;
			current_file = current_file->next;
		}
		current_cmd = current_cmd->next;
	}
}
