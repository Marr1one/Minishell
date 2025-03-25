/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:30:52 by maissat           #+#    #+#             */
/*   Updated: 2025/03/25 17:21:19 by braugust         ###   ########.fr       */
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

char *expand_string(const char *arg, t_data *data)
{
    int i, j, k, start, var_len;
    int final_len = 0;
    char *result;
    char *var_name;
    char *var_value;
    char *exit_str;

    /* Réinitialisation de l'état de quote */
    data->in_quote = 0;
    i = 0;
    while (arg[i])
    {
        if (handle_quotes(arg[i], data))
        {
            final_len++;  /* On copie la quote telle quelle */
            i++;
        }
        else if (arg[i] == '$' && data->in_quote != 2)
        {
            i++; /* On saute le '$' */
            if (arg[i] && arg[i] == '?')
            {
                exit_str = ft_itoa(data->exit_status);
                if (!exit_str)
                    exit_str = "";
                final_len += (int)ft_strlen(exit_str);
                free(exit_str);
                i++; /* On saute le '?' */
            }
            else
            {
                start = i;
                while (arg[i] && (isalnum((unsigned char)arg[i]) || arg[i] == '_'))
                    i++;
                var_len = i - start;
                if (var_len > 0)
                {
                    var_name = ft_malloc(var_len + 1);
                    if (!var_name)
                        return (NULL);
                    for (k = 0; k < var_len; k++)
                        var_name[k] = arg[start + k];
                    var_name[var_len] = '\0';
                }
                else
                    var_name = NULL;
                if (var_name)
                    var_value = getenv(var_name);
                else
                    var_value = NULL;
                if (!var_value)
                    var_value = "";
                final_len += (int)ft_strlen(var_value);
                if (var_name)
                    free(var_name);
            }
        }
        else
        {
            final_len++;
            i++;
        }
    }

    /* Allocation de la chaîne résultat */
    result = ft_malloc(final_len + 1);
    if (!result)
        return (NULL);

    /* Deuxième passe : construction de la chaîne résultat */
    i = 0;
    j = 0;
    data->in_quote = 0; /* Réinitialisation avant la reconstruction */
    while (arg[i])
    {
        if (handle_quotes(arg[i], data))
        {
            result[j++] = arg[i++];
        }
        else if (arg[i] == '$' && data->in_quote != 2)
        {
            i++; /* On saute le '$' */
            if (arg[i] && arg[i] == '?')
            {
                exit_str = ft_itoa(data->exit_status);
                for (k = 0; exit_str[k]; k++)
                    result[j++] = exit_str[k];
                free(exit_str);
                i++; /* On saute le '?' */
            }
            else
            {
                start = i;
                while (arg[i] && (isalnum((unsigned char)arg[i]) || arg[i] == '_'))
                    i++;
                var_len = i - start;
                if (var_len > 0)
                {
                    var_name = ft_malloc(var_len + 1);
                    if (!var_name)
                    {
                        free(result);
                        return (NULL);
                    }
                    for (k = 0; k < var_len; k++)
                        var_name[k] = arg[start + k];
                    var_name[var_len] = '\0';
                }
                else
                    var_name = NULL;
                if (var_name)
                    var_value = getenv(var_name);
                else
                    var_value = NULL;
                if (!var_value)
                    var_value = "";
                for (k = 0; var_value[k]; k++)
                    result[j++] = var_value[k];
                if (var_name)
                    free(var_name);
            }
        }
        else
        {
            result[j++] = arg[i++];
        }
    }
    result[j] = '\0';
    return result;
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
