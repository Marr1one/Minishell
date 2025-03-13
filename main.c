/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/03/13 17:05:07 by maissat          ###   ########.fr       */
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


t_token	*tokenizer(char *input)
{
	int			i;
	t_token		*list;
	t_type		expect;
	t_type		redirect;
	int			start;

	i = 0;
	expect = CMD;
	list = NULL;
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (input[i] == '\0')
		{
			printf("dans le cas ou il nya que des espaces!\n");
			break;
		}
		if (is_space(input[i]) == 0 || is_redirect(input[i]) == 0 || is_pipe(input[i]) == 0)
		{
			start = i;
			while(is_space(input[i]) == 0 || is_redirect(input[i]) == 0 || is_pipe(input[i]) == 0)
				i++;
			list = add_node(ft_substr(input, start, i), list, expect);
			if (expect == CMD)
				expect = ARG;
			if (expect == FICHIER)
				expect = ARG;
			continue;
		}
		//else if (input[i] == '-')
		//{
		//	// Traiter le tiret et ce qui suit comme un token distinct
		//	start = i++;  // Avancer après le tiret
		//	while(is_alpha(input[i]) == 1 || is_digit(input[i]) == 1)
		//		i++;
		//	list = add_node(ft_substr(input, start, i), list, ARG);  // Toujours un ARG
		//	continue;
		//}
		if (input[i] == '|')
		{
			if (case_pipe(input, i) > 1)
				break;
			list = add_node("|", list, PIPE);
			expect = CMD;
			i++;
			continue;
		}
		if (input[i] == '<' || input[i] == '>')
		{
			redirect = case_redirect(input, i);
			if (redirect == UNKNOWN)
			{
				printf("erreur de redirect\n");
				break;
			}
			if (input[i] == '<')
				list = add_node("<", list, redirect);
			else
				list = add_node(">", list, redirect);
			if (redirect == INFILE || redirect == OUTFILE_TRUNC)
                i++;
            else if (redirect == HEREDOC || redirect == OUTFILE_APPEND)
                i += 2;
			expect = FICHIER;
			continue;
		}
		// if ()
		i++;
	}
	return (list);
}

int		count_arguments(t_token *list)
{
	int		count;
	t_token	*current;

	count = 0;
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
	while (current_tkn)
	{
		
		if (current_tkn->type == PIPE)
		{
			current_cmd->args[i] = NULL;
			i = 0;
			current_cmd = current_cmd->next;
		}
		if (current_tkn->type == CMD || current_tkn->type == ARG)
		{
			if (current_tkn->type == CMD)
				current_cmd->args = ft_malloc(sizeof(char *) * (count_arguments(current_tkn) + 1));
			current_cmd->args[i] = ft_strdup(current_tkn->content);
			i++;
		} 
		current_tkn = current_tkn->next;
	}
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
	count = 0;
	current = list;
	while (current)
	{
		if (current->type == CMD)
		{
			list_cmd = add_cmd_node(list_cmd);
			count++;
		}
		current = current->next;
	}
	printf("count = %d\n", count);
	return (list_cmd);
}

void	show_list_cmd(t_cmd *list)
{
	t_cmd *current;
	int		i;

	current = list;
	i = 0;
	while (current)
	{
		printf("commande %d\n", i +1);
		i++;
		show_tab(current->args);
		current = current->next;
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
		show_list_cmd(list_cmd);
		//printf("test dune commande\n");
		//show_tab(args_test);
    }
    return (0);
}
