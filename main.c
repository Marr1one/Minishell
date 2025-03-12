/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/03/12 03:28:03 by maissat          ###   ########.fr       */
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
	printf("in case pipe\n");
	int	count;
	
	count = 0;
	while(input[i] == '|')
	{
		count++;
		i++;
	}
	return (count);
	
}

t_token	*tokenizer(char *input)
{
	int			i;
	t_token		*list;
	int			start;

	i = 0;
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
		if (is_alpha(input[i]) == 0)
		{
			start = i;
			while(is_alpha(input[i]) == 0)
				i++;
			list = add_node(ft_substr(input, start, i), list, WORD);
		}
		if (input[i] == '|')
		{
			if (case_pipe(input, i) > 1)
				break;
			list = add_node("|", list, PIPE);
		}
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
