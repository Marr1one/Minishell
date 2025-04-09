/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:13:09 by braugust          #+#    #+#             */
/*   Updated: 2025/04/09 15:54:06 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	size_t	len;
	char	*dup;

	len = 0;
	while (s[len] && len < n)
		len++;

	dup = ft_malloc(len + 1);
	if (!dup)
		return (NULL);

	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int handle_variable_expansion(const char *input, t_data *data, int *i)
{
    int start;
    int var_len;
    char *var_name;
    char *var_value;
    int len;

    start = *i;
    while (input[*i] && (isalnum((unsigned char)input[*i]) || input[*i] == '_'))
    {
        (*i)++;
    }
    
    var_len = *i - start;
    var_name = ft_strndup(input + start, var_len);
    var_value = check_env(data, var_name);
    
    if (!var_value)
        var_value = "";
    
    len = strlen(var_value);
    return (len);
}

int process_dollar_expansion(const char *input, t_data *data, int *i)
{
    int dollar_count;
    int len;
    char *exit_str;

    dollar_count = 0;
    len = 0;
    while (input[*i] == '$')
    {
        dollar_count++;
        (*i)++;
	}
    len += dollar_count / 2;
    if (dollar_count % 2 == 0)
        return (len);
    if (input[*i] == '?')
    {
        exit_str = ft_itoa(data->exit_status);
        len += strlen(exit_str);
        (*i)++;
    }
    else if (input[*i] && (isalpha((unsigned char)input[*i]) || input[*i] == '_'))
        len += handle_variable_expansion(input, data, i);
    else
        len++;
    return (len);
}

int calc_expanded_len(const char *input, t_data *data)
{
    int i;
    int len;

    i = 0;
    len = 0;
    data->in_quote = 0;
    while (input[i])
    {
        if (handle_quotes(input[i], data))
        {
            i++;
            continue;
        }
        if (input[i] == '$' && data->in_quote != 2)
        {
            len += process_dollar_expansion(input, data, &i);
            continue;
        }
        len++;
        i++;
    }
    return (len);
}

void handle_dollar_counting(char *result, const char *input, t_idx *idx, int *dollar_count)
{
    int k;
    
    *dollar_count = 0;
    while (input[*(idx->i)] == '$')
    {
        (*dollar_count)++;
        (*(idx->i))++;
    }
    
    k = -1;
    while (++k < *dollar_count / 2)
    {
        result[*(idx->j)] = '$';
        (*(idx->j))++;
    }
}

void handle_exit_or_dollar(char *result, const char *input, t_data *data, t_idx *idx)
{
    char *exit_str;
    int k;
    
    if (input[*(idx->i)] == '?')
    {
        exit_str = ft_itoa(data->exit_status);
        k = 0;
        while (exit_str[k])
        {
            result[*(idx->j)] = exit_str[k];
            (*(idx->j))++;
            k++;
        }
        (*(idx->i))++;
    }
    else
    {
        result[*(idx->j)] = '$';
        (*(idx->j))++;
    }
}

void process_variable_value(char *result, char *var_value, int *j)
{
	int k;
    
    if (!var_value)
	var_value = "";
	
    k = 0;
    while (var_value[k])
    {
		result[*j] = var_value[k];
        (*j)++;
        k++;
    }
}

void process_dollar_and_variables(char *result, const char *input, t_data *data, t_idx *idx)
{
	int dollar_count;
	int start;
	int var_len;
	char *var_name;
	char *var_value;
	
	handle_dollar_counting(result, input, idx, &dollar_count);
	if (dollar_count % 2 == 0)
		return;
	if (input[*(idx->i)] == '?' || !(input[*(idx->i)] && 
		(isalpha((unsigned char)input[*(idx->i)]) || input[*(idx->i)] == '_')))
	{
		handle_exit_or_dollar(result, input, data, idx);
		return;
	}
	start = *(idx->i);
	while (input[*(idx->i)] && 
		   (isalnum((unsigned char)input[*(idx->i)]) || input[*(idx->i)] == '_'))
		(*(idx->i))++;
	var_len = *(idx->i) - start;
	var_name = ft_strndup(input + start, var_len);
	var_value = check_env(data, var_name);
	process_variable_value(result, var_value, idx->j);
}


void process_regular_char(char *result, const char *input, t_data *data, t_idx *idx)
{
    if (handle_quotes(input[*(idx->i)], data))
    {
        (*(idx->i))++;
        return;
    }
    if (input[*(idx->i)] == '$' && data->in_quote != 2)
    {
        process_dollar_and_variables(result, input, data, idx);
        return;
    }
    
    result[*(idx->j)] = input[*(idx->i)];
    (*(idx->i))++;
    (*(idx->j))++;
}

void build_expanded_string(char *result, const char *input, t_data *data)
{
    int i;
    int j;
    t_idx idx;
    
    i = 0;
    j = 0;
    idx.i = &i;
    idx.j = &j;
    data->in_quote = 0;
    
    while (input[i])
    {
        process_regular_char(result, input, data, &idx);
    }
    
    result[j] = '\0';
}

char	*expand_string(const char *input, t_data *data)
{
	char	*result;
	int		final_len;

	if (!input)
		return (NULL);
	final_len = calc_expanded_len(input, data);
	result = ft_malloc(final_len + 1);
	if (!result)
		return (NULL);
	build_expanded_string(result, input, data);
	return (result);
}

void	expand_argument(char **arg, t_data *data)
{
	if (!arg || !(*arg))
		return ;
	*arg = expand_string(*arg, data);
}

void	expand_cmd_args(t_cmd *cmd, t_data *data)
{
	int	i;
	
	i = 0;
	if (!cmd || !cmd->args)
		return ;
	while (cmd->args[i])
	{
		expand_argument(&cmd->args[i], data);
		i++;
	}
}

void	expand_file_path(t_file *file, t_data *data)
{
	if (!file)
		return ;
	if (file->mode == HEREDOC)
	{
		file->path = quoteless_string(file->path);
		return;
	}
	file->path = expand_string(file->path, data);
}

void	expand_cmd_files(t_cmd *cmd, t_data *data)
{
	t_file	*cur;

	cur = cmd->files;
	while (cur)
	{
		expand_file_path(cur, data);
		cur = cur->next;
	}
}

void	expand_all(t_cmd *cmd, t_data *data)
{
	t_cmd	*cur;
	
	cur = cmd;
	while (cur)
	{
		expand_cmd_args(cur, data);
		expand_cmd_files(cur, data);
		cur = cur->next;
	}
}
