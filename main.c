/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/03/21 21:34:21 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_substr_qte(char *str,  int start, int len)
{
	int	i;
	char	*new_str;

	i = 0;
	if (!str)
		return (NULL);
	new_str = ft_malloc(sizeof(char) * (len + 1));
	if (new_str == NULL)
		return (NULL);
	while (str[start] && i < len)
	{
		new_str[i++] = str[start++];
	}
	new_str[i] = '\0';
	return (new_str);
}

char	*ft_substr(char *str,  int start, int end)
{
	int	i;
	char	*new_str;

	i = 0;
	if (!str || start >= end)
		return (NULL);
	new_str = ft_malloc(sizeof(char) * (end - start + 1));
	if (new_str == NULL)
		return (NULL);
	while (str[start] && start < end)
	{
		new_str[i++] = str[start++];
	}
	new_str[i] = '\0';
	return (new_str);
}


int		count_arguments(t_token *list)
{
	int		count;
	t_token	*current;

	count = 0;
	if (!list)
		return (0);
	if (list->type == PIPE)
		current = list->next;
	else
		current = list;
	while (current)
	{
		if (current->type == PIPE)
			break;
		if (current->type == CMD || current->type == ARG)
			count++;
		current = current->next;
	}
	return (count);
}


t_cmd	*create_args(t_token *list_tkn, t_cmd *list_cmd)
{
	int		i;
	t_token	*current_tkn;
	t_cmd	*current_cmd;

	current_tkn = list_tkn;
	current_cmd = list_cmd;
	i = 0;
	current_cmd->args = ft_malloc(sizeof(char *) * (count_arguments(current_tkn) + 1));
	while (current_tkn && current_cmd)
	{
		if (current_tkn->type == CMD || current_tkn->type == ARG)
		{
			current_cmd->args[i++] = current_tkn->content;
		} 
		else if (current_tkn->type == PIPE)
		{
			current_cmd->args[i] = NULL;	
			i = 0;
			current_cmd = current_cmd->next;
			current_cmd->args = ft_malloc(sizeof(char *) * (count_arguments(current_tkn) + 1));
		}
		current_tkn = current_tkn->next;
	}
	if (current_cmd)
		current_cmd->args[i] = NULL;
	return (list_cmd);
}

t_cmd	*add_cmd_node(t_cmd *cmd_list)
{
	t_cmd *new_cmd;
	t_cmd *last;

	new_cmd = ft_malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	
	new_cmd->args = NULL;
	new_cmd->files = NULL; //mettre plutot b_zero
	new_cmd->next = NULL;
	if (!cmd_list)
		return (new_cmd);
	else
	{
		last = cmd_list;
		while (last->next)
			last = last->next;
		last->next = new_cmd;
	}
	return (cmd_list);
}


t_cmd	*parse_cmd(t_token *list)
{
	t_token *current;
	t_cmd	*list_cmd;
	int		count;

	count = 0;
	list_cmd = NULL;
	list_cmd = add_cmd_node(list_cmd);
	current = list;
	while (current)
	{
		if (current->type == PIPE)
		{
			if (!current->next || current->next->type == PIPE)
			{
				printf("Syntax error: pipe not closed\n");
				return (NULL);
			}
			list_cmd = add_cmd_node(list_cmd);
			count++;
		}
		current = current->next;
	}
	return (list_cmd);
}


t_file *	add_file(t_file	*list_file, t_token *file_tkn, t_type mode)
{
	t_file *new_file;
	t_file *last_file;

	new_file = ft_malloc(sizeof(t_file));
	if (!new_file)
		return (NULL);
	new_file->mode = mode;
	new_file->path = file_tkn->content;
	new_file->next = NULL;
	if (list_file == NULL)
		return (new_file);
	else
	{
		last_file = list_file;
		while(last_file->next)
			last_file = last_file->next;
		last_file->next = new_file;	
	}
	return (list_file);
}

t_type save_mode(t_token current_tkn)
{
	t_type save;

	save = UNKNOWN;
	if (current_tkn.type == HEREDOC)
			save = HEREDOC;
	if (current_tkn.type == INFILE)
			save = INFILE;
	if (current_tkn.type == OUTFILE_APPEND)
			save = OUTFILE_APPEND;
	if (current_tkn.type == OUTFILE_TRUNC)
			save = OUTFILE_TRUNC;
	return (save);
}

// int	is_tkn_redirect(t_token *current_tkn)
// {
// 	if (current_tkn->type == HEREDOC || current_tkn->type == INFILE || 
// 		current_tkn->type == OUTFILE_APPEND || current_tkn->type == OUTFILE_TRUNC)
// 		return (1);
// 	return (0);
// }

t_cmd *create_files(t_token *list_tkn, t_cmd *list_cmd)
{
	t_token *current_tkn;
	t_cmd 	*current_cmd;
	t_type	save;

	current_tkn = list_tkn;
	save = UNKNOWN;
	current_cmd = list_cmd;
	while (current_tkn && current_cmd)
	{
		if (is_redirect(current_tkn->content[0]))
			save = save_mode(*current_tkn);
		if (current_tkn->type == FICHIER)
		{
			current_cmd->files = add_file(current_cmd->files, current_tkn, save);
			save = UNKNOWN;
		}
		if (current_tkn->type == PIPE)
		{
			current_cmd = current_cmd->next;
			save = UNKNOWN;
		}
		current_tkn = current_tkn->next;
	}
	return (list_cmd);
}

char	*quoteless_string(char *str)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	while(str[i])
	{
		if (str[i] != '"' && str[i] != '\'')
			j++;
		i++;
	}
	new_str = ft_malloc(sizeof(char) * (j + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '"' && str[i] != '\'')
		{
			new_str[j++] = str[i];
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

void	remove_quotes(t_token *list_tkn)
{
	t_token *current_tkn;

	current_tkn = list_tkn;
	while (current_tkn)
	{
		current_tkn->content = quoteless_string(current_tkn->content);
		current_tkn = current_tkn->next;
	}
}

int main(int argc, char **argv, char **envp)
{
	
    (void)argv;
	(void)envp;
	char	*input;
	char	**paths;
	t_cmd	*list_cmd;
	t_token *list_tkn;
	t_data	data;
	
    data.envp = copy_env(envp);
	data.exit_status = 0;
    if (argc != 1)
        return (printf("Usage : ./minishell\n"));
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);

    while (1)
    {
        input = readline("\033[0;34mMini_\033[0;31mshell$\033[0m ");
        if (!input)
            break;
		input = ft_strdup(input);
        add_history(input);
		list_tkn = tokenizer(input);
		show_list(list_tkn);
		remove_quotes(list_tkn);
		// rm_qts_nodes(&data);
		list_cmd = parse_cmd(list_tkn);
		if (list_cmd == NULL)
        {
            free(input);
            continue;
        }
		list_cmd = create_args(list_tkn, list_cmd);
		list_cmd = create_files(list_tkn, list_cmd);
		// expand_var_command(list_cmd, data.exit_status, &data);
		paths = ft_split(get_path_env(envp), ':');
    	paths = add_slash_all(paths);
		expand_all(list_cmd, &data);
		execute_cmds(&data, list_cmd, paths);
    }
    return (0);
}

