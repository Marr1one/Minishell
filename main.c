/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/04/02 15:45:06 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_substr_qte(char *str,  int start, int len)
{
	int		i;
	char	*new_str;

	i = 0;
	if (!str)
		return (NULL);
	new_str = ft_malloc(sizeof(char) * (len + 1));
	if (new_str == NULL)
		return (NULL);
	while (str[start] && i < len)
		new_str[i++] = str[start++];
	new_str[i] = '\0';
	return (new_str);
}

char	*ft_substr(char *str,  int start, int end)
{
	int		i;
	char	*new_str;

	i = 0;
	if (!str || start >= end)
		return (NULL);
	new_str = ft_malloc(sizeof(char) * (end - start + 1));
	if (new_str == NULL)
		return (NULL);
	while (str[start] && start < end)
		new_str[i++] = str[start++];
	new_str[i] = '\0';
	return (new_str);
}


int	count_arguments(t_token *list)
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
			current_cmd->args[i++] = current_tkn->content;
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
	t_cmd	*new_cmd;
	t_cmd	*last;

	new_cmd = ft_malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	
	ft_memset(new_cmd, 0, sizeof(t_cmd));	
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
	t_token	*current;
	t_cmd	*list_cmd;

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
		}
		current = current->next;
	}
	return (list_cmd);
}


t_file *	add_file(t_file	*list_file, t_token *file_tkn, t_type mode)
{
	t_file	*new_file;
	t_file	*last_file;

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

t_type	save_mode(t_token current_tkn)
{
	t_type	save;

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

void	remove_quotes(t_cmd *list_cmd)
{
	t_cmd	*current_cmd;
	int	i;

	i = 0;
	current_cmd = list_cmd;
	while (current_cmd)
	{
		i = 0;
		while (current_cmd->args && current_cmd->args[i])
		{
            current_cmd->args[i] = quoteless_string(current_cmd->args[i]);
			i++;
		}
		current_cmd = current_cmd->next;
    }	
}

// char *quoteless_string(char *str)
// {
//     int     i;
//     int     j;
//     char    *new_str;
//     int     in_dquote;
//     int     in_squote;

//     i = 0;
//     j = 0;
//     in_dquote = 0;
//     in_squote = 0;
    
//     while (str[i])
//     {
//         if (str[i] == '"' && !in_squote)
//             in_dquote = !in_dquote;
//         else if (str[i] == '\'' && !in_dquote)
//             in_squote = !in_squote;
//         else
//             j++;
//         i++;
//     }
    
//     new_str = ft_malloc(sizeof(char) * (j + 1));
//     if (!new_str)
//         return (NULL);
    
//     i = 0;
//     j = 0;
//     in_dquote = 0;
//     in_squote = 0;
    
//     while (str[i])
//     {
//         if (str[i] == '"' && !in_squote)
//             in_dquote = !in_dquote;
//         else if (str[i] == '\'' && !in_dquote)
//             in_squote = !in_squote;
//         else
//             new_str[j++] = str[i];
//         i++;
//     }
    
//     new_str[j] = '\0';
//     return (new_str);
// }

//Calcule la longueur de la chaîne sans les guillemets

int	calculate_length_without_quotes(char *str)
{
	int	i;
	int	j;
	int	in_dquote;
	int	in_squote;

	i = 0;
	j = 0;
	in_dquote = 0;
	in_squote = 0;
	while (str[i])
	{
		if (str[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		else if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else
			j++;
		i++;
	}
	return (j);
}

// Remplit la nouvelle chaîne sans les guillemets
char	*fill_string_without_quotes(char *new_str, char *str)
{
	int	i;
	int	j;
	int	in_dquote;
	int	in_squote;

	i = 0;
	j = 0;
	in_dquote = 0;
	in_squote = 0;
	while (str[i])
	{
		if (str[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		else if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else
			new_str[j++] = str[i];
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

//Fonction principale qui enlève les guillemets d'une chaîne
char	*quoteless_string(char *str)
{
	int		length;
	char	*new_str;

	length = calculate_length_without_quotes(str);
	new_str = ft_malloc(sizeof(char) * (length + 1));
	if (!new_str)
		return (NULL);
	new_str = fill_string_without_quotes(new_str, str);
	return (new_str);
}

void initialize_data(t_data *data, char **envp)
{
    ft_memset(data, 0, sizeof(*data));
    data->envp = copy_env(envp);
	data->gc = get_gc();
}

/* Configure les signaux */
void setup_signals(void)
{
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}

int check_arguments(int argc)
{
	if (argc != 1)
	{
		printf("Usage : ./minishell\n");
		return (0);
	}
	return (1);
}

/* Prépare et traite une commande */
void process_command(char *input, t_data *data)
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
	//show_list_cmd(list_cmd);
	expand_all(list_cmd, data);
	remove_quotes(list_cmd);
    execute_cmds(data, list_cmd);
}

/* Boucle principale du shell */
void shell_loop(t_data *data)
{
    char *input;	

    while (1)
    {
        input = readline("\033[0;32mminishell\033[0m ");
        if (!input)
		{
			free_all(data->gc);
			free(input);
            break;
		}
        add_history(input);
        process_command(input, data);
		//printf("exit status = %d\n", data->exit_status);
        free(input);
    }
}

/* Fonction principale */
int main(int argc, char **argv, char **envp)
{
	t_data data;

	(void)argv;
	if (!check_arguments(argc))
		return (1);
	initialize_data(&data, envp);
	setup_signals();
	shell_loop(&data);
	return (0);
}
