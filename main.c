/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/03/14 15:29:36 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_substr(char *str,  int start, int end)
{
	int	i;
	char	*new_str;

	i = 0;
	if (!str)
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

t_token	*add_node(char *str, t_token *list,  t_type type)
{
	t_token *new_node;

	new_node = ft_malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->next = NULL;
	new_node->content = ft_strdup(str);
	new_node->type = type;
	if (!list)
		list = new_node;
	else
		findlast_token(list)->next = new_node;
	return (list);
}

int	case_pipe(char *input, int	i)
{	
	int	count;
	
	count = 0;
	while(input[i] == '|')
	{
		count++;
		i++;
	}
	return (count);
	
}

t_type	case_redirect(char *input, int	i)
{
	int	count;
	
	count = 0;
	if (input[i] == '<')
	{
		while (input[i] == '<')
		{
			i++;
			count++;
		}
		if (count == 1)
			return (INFILE);
		if (count == 2)
			return (HEREDOC);
		return (UNKNOWN);
	}
	else
	{
		while (input[i] == '>')
		{
			i++;
			count++;
		}
		if (count == 1)
			return (OUTFILE_TRUNC);
		if (count == 2)
			return (OUTFILE_APPEND);
		return (UNKNOWN);
	}
}
// is_redirect && is_pipe && is_space

int is_word(char c)
{
	if (is_space(c))
		return (0);
	if (is_redirect(c))
		return (0);
	if (is_pipe(c))
		return (0);
	return (1);
}

int		count_arguments(t_token *list)
{
	int		count;
	t_token	*current;

	count = 0;
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
	printf("nombre darguments = %d\n", count);
	return (count);
}


t_cmd	*create_args(t_token *list_tkn, t_cmd *list_cmd)
{
	printf("dans create args\n");
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
			current_cmd->args[i++] = ft_strdup(current_tkn->content);
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
	new_cmd->files = NULL;
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

	list_cmd = NULL;
	list_cmd = add_cmd_node(list_cmd);
	count = 1;
	current = list;
	while (current)
	{
		if (current->type == PIPE)
		{
			list_cmd = add_cmd_node(list_cmd);
			count++;
		}
		current = current->next;
	}
	printf("count = %d\n", count);
	return (list_cmd);
}

t_file *	add_file(t_file	*list_file, t_token *file_tkn)
{
	t_file *new_file;
	t_file *last_file;

	new_file = ft_malloc(sizeof(t_file));
	if (!new_file)
		return (NULL);
	new_file->mode = UNKNOWN;
	new_file->path = ft_strdup(file_tkn->content);
	new_file->next = NULL;
	if (list_file == NULL)
		return (new_file);
	else
		last_file = list_file;
		while(last_file->next)
			last_file = last_file->next;
		last_file->next = new_file;
	return (list_file);
}

t_cmd *create_files(t_token *list_tkn, t_cmd *list_cmd)
{
	t_token *current_tkn;
	t_cmd *current_cmd;

	current_tkn = list_tkn;
	current_cmd = list_cmd;
	while (current_tkn && current_cmd)
	{
		if (current_tkn->type == FICHIER)
		{
			current_cmd->files = add_file(current_cmd->files, current_tkn);
		}
	}
}

int main(int argc, char **argv, char **envp)
{
	
    (void)argv;
	(void)envp;
	char	*input;
	t_cmd	*list_cmd;
	t_token *list_tkn;
    
    if (argc != 1)
        return (printf("Usage : ./minishell\n"));
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);

    while (1)
    {
        input = readline("\033[0;32mminishell$\033[0m ");
        if (!input)
            break;
        add_history(input); 
		list_tkn = tokenizer(input);
		printf("list apres tokenize\n");
		show_list(list_tkn);
		list_cmd = parse_cmd(list_tkn);
		list_cmd = create_args(list_tkn, list_cmd);
		list_cmd = create_files(list_tkn, list_cmd);
		show_list_cmd(list_cmd);
		//printf("test dune commande\n");
		//show_tab(args_test);
    }
    return (0);
}
