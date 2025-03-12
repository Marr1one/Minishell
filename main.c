/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/03/12 22:16:07 by maissat          ###   ########.fr       */
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
	// if (start >= len_str)
	// 	return (ft_strdup(""));
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
		if (is_alpha(input[i]) == 1 || is_digit(input[i]) == 1
			|| input[i] == '/')
		{
			start = i;
			while(is_alpha(input[i]) == 1 || is_digit(input[i]) == 1
				|| input[i] == '/' || input[i] == '-')
				i++;
			list = add_node(ft_substr(input, start, i), list, expect);
			if (expect == CMD)
				expect = ARG;
			if (expect == FICHIER)
				expect = ARG;
			continue;	
		}
		else if (input[i] == '-')
		{
			// Traiter le tiret et ce qui suit comme un token distinct
			start = i++;  // Avancer après le tiret
			while(is_alpha(input[i]) == 1 || is_digit(input[i]) == 1)
				i++;
			list = add_node(ft_substr(input, start, i), list, ARG);  // Toujours un ARG
			continue;
		}
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

int main(int argc, char **argv, char **envp)
{
	
    (void)argv;
	(void)envp;
	char	*input;
	t_token *list;
    
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
		list = tokenizer(input);
		show_list(list);
    }
    return (0);
}
