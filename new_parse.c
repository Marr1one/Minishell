/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:30:52 by maissat           #+#    #+#             */
/*   Updated: 2025/03/26 03:13:56 by braugust         ###   ########.fr       */
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

int calc_final_len(const char *arg, t_data *data)
{
	int i;
	int len;

	i = 0;
	len = 0;
	data->in_quote = 0;
	while (arg[i])
	{
		if (handle_quotes(arg[i], data))
			len++;
		else if (arg[i] == '$' && data->in_quote != 2)
			len += 1;
		else
			len++;
		i++;
	}
	return (len);
}

void	append_var_value(char *result, int *j, char *var_name, t_data *data)
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
	}
	else
	{
		var_value = getenv(var_name);
		if (!var_value)
			var_value = "";
		k = 0;
		while (var_value[k])
			result[(*j)++] = var_value[k++];
	}
}

void handle_dollar(char *result, const char *arg, t_idx *idx, t_data *data)
{
    int		dollar_count;
    char	*var_name;
    int		k;

    dollar_count = get_dollar_count(arg, &idx->i);
    k = dollar_count / 2;
    while (k-- > 0)
        result[idx->j++] = '$';
    if (dollar_count % 2 != 0)
    {
        var_name = extract_var_name(arg, &idx->i);
        if (var_name)
        {
            append_var_value(result, &idx->j, var_name, data);
            free(var_name);
        }
    }
}


void	build_final_string(char *result, const char *arg, t_data *data)
{
    t_idx idx;

    idx.i = 0;
    idx.j = 0;
    data->in_quote = 0;
    while (arg[idx.i])
    {
        if (handle_quotes(arg[idx.i], data))
            result[idx.j++] = arg[idx.i++];
        else if (arg[idx.i] == '$' && data->in_quote != 2)
            handle_dollar(result, arg, &idx, data);
        else
            result[idx.j++] = arg[idx.i++];
    }
    result[idx.j] = '\0';
}


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

char	*expand_string(const char *arg, t_data *data)
{
	char	*result;
	int	final_len;

	final_len = calc_final_len(arg, data);
	result = ft_malloc(final_len + 1);
	if (!result)
		return (NULL);
	build_final_string(result, arg, data);
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
