/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:13:09 by braugust          #+#    #+#             */
/*   Updated: 2025/04/09 00:08:36 by braugust         ###   ########.fr       */
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

int	calc_expanded_len(const char *input, t_data *data)
{
	int	i = 0;
	int	len = 0;

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
			int	dollar_count = 0;
			while (input[i] == '$')
			{
				dollar_count++;
				i++;
			}
			len += dollar_count / 2;
			if (dollar_count % 2 == 0)
			{
				continue;
			}
			if (input[i] == '?')
			{
				char *exit_str = ft_itoa(data->exit_status);
				len += strlen(exit_str);
				i++;
			}
			else if (input[i] &&
				(isalpha((unsigned char)input[i]) || input[i] == '_'))
			{
				int	start = i;
				while (input[i] &&
					(isalnum((unsigned char)input[i]) || input[i] == '_'))
				{
					i++;
				}
				{
					char *var_name = ft_strndup(input + start, i - start);
					char *var_value = check_env(data, var_name);
					if (!var_value)
						var_value = "";
					len += strlen(var_value);
				}
			}
			else
			{
				len++;
			}
			continue;
		}
		len++;
		i++;
	}
	return (len);
}


void	build_expanded_string(char *result, const char *input, t_data *data)
{
	int	i = 0;
	int	j = 0;
	int k;
	int	dollar_count;
	char *exit_str;
	int	start;
	int var_len;
	char *var_name;
	char *var_value;
	
	data->in_quote = 0;
	exit_str = NULL;
	while (input[i])
	{
		if (handle_quotes(input[i], data))
		{
			i++;
			continue;
		}
		if (input[i] == '$' && data->in_quote != 2)
		{
			dollar_count = 0;
			while (input[i] == '$')
			{
				dollar_count++;
				i++;
			}
			k = -1;
			while (++k < dollar_count / 2)
				result[j++] = '$';
			if (dollar_count % 2 == 0)
				continue;
			if (input[i] == '?')
			{
				exit_str = ft_itoa(data->exit_status);
				k = 0;
				while (exit_str[k])
					result[j++] = exit_str[k++];
				i++;
			}
			else if (input[i] &&
				(isalpha((unsigned char)input[i]) || input[i] == '_'))
			{
				start = i;
				while (input[i] &&
					(isalnum((unsigned char)input[i]) || input[i] == '_'))
					i++;
				var_len = i - start;
				{
					var_name = ft_strndup(input + start, var_len);
					var_value = check_env(data, var_name);
					if (!var_value)
						var_value = "";
					k = 0;
					while (var_value[k])
						result[j++] = var_value[k++];
				}
			}
			else
				result[j++] = '$';
			continue;
		}
		result[j++] = input[i++];
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
