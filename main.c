/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/02/06 00:27:19 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		printf("%s\n", tab[i]);
		i++;
	}
}

int	main(int argc, char **argv , char **envp)
{
	
	(void)argv;
	t_data data;

	if (argc != 1)
		return (printf("Usage : ./minishell\n"));
	ft_memset(&data, 0, sizeof(t_data));
	data.envp = copy_env(envp);
	signal(SIGINT, sigint_handler);
	while (1)
	{
		data.input = readline("\033[0;32mminishell$\033[0m ");
		if (!data.input)
			break ;
		data.args = ft_split(data.input, ' ');
			//show_tab(data.args);
		data.list = add_chained_list(&data);
		if (case_redirection(&data, data.envp) == 1)
		{
			// printf("on  va rentrer dans parsing car on trouve pas >\n");
			parsing(data.input, data.envp, &data);
		}
		add_history(data.input);						
	}
	return (0);
}