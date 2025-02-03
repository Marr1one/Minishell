/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/02/03 18:14:20 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		printf("tab[%d] = %s\n", i, tab[i]);
		i++;
	}
}

int	main(int argc, char **argv , char **envp)
{
	(void)argv;
	(void)argc;
	t_data data;

	ft_memset(&data, 0, sizeof(t_data));
	signal(SIGINT, sigint_handler);
	while (1)
	{
		data.input = readline("\033[0;32mminishell$\033[0m ");
		if (!data.input)
			break ;
		data.args = ft_split(data.input, ' ');
		//show_tab(data.args);
		add_chained_list(&data);
		if (case_redirection(&data, envp) == 1)
		{
			// printf("on va rentrer dans parsing car on trouve pas >\n");
			parsing(data.input, envp, &data);
		}
		add_history(data.input);								
	}
	return (0);
}