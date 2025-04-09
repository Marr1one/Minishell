/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   submain.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:27:55 by braugust          #+#    #+#             */
/*   Updated: 2025/04/09 16:28:10 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

void	initialize_data(t_data *data, char **envp)
{
	ft_memset(data, 0, sizeof(*data));
	data->envp = copy_env(envp);
	data->gc = get_gc();
}

int	check_arguments(int argc)
{
	if (argc != 1)
	{
		printf("Usage : ./minishell\n");
		return (0);
	}
	return (1);
}

int	count_without_empty(char **args)
{
	int		count;
	int		i;
	char	**test_args;

	i = 0;
	count = 0;
	test_args = args;
	while (args[i])
	{
		if (args[i][0] != '\0')
			count++;
		i++;
	}
	return (count);
}

void	skip_empty_args(t_cmd *list_cmd)
{
	t_cmd	*curr_cmd;
	int		i;
	int		j;
	char	**new_args;
	int		c;

	curr_cmd = list_cmd;
	while (curr_cmd)
	{
		j = 0;
		i = 0;
		c = count_without_empty(curr_cmd->args) + 1;
		new_args = ft_malloc(sizeof(char *) * c);
		if (!new_args)
			return ;
		while (curr_cmd->args[i])
		{
			if (curr_cmd->args[i][0] != '\0')
				new_args[j++] = curr_cmd->args[i];
			i++;
		}
		new_args[j] = NULL;
		curr_cmd->args = new_args;
		curr_cmd = curr_cmd->next;
	}
}

void	process_command(char *input, t_data *data)
{
	t_token	*list_tkn;
	t_cmd	*list_cmd;

	list_tkn = tokenizer(input, data);
	if (list_tkn == NULL)
		return ;
	list_cmd = parse_cmd(list_tkn);
	if (list_cmd == NULL)
		return ;
	list_cmd = create_args(list_tkn, list_cmd);
	list_cmd = create_files(list_tkn, list_cmd);
	expand_all(list_cmd, data);
	skip_empty_args(list_cmd);
	remove_quotes(list_cmd);
	data->list = list_tkn;
	execute_cmds(data, list_cmd);
}
