/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/01/29 17:25:25 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv , char **envp)
{
	(void)argv;
	(void)argc;
	t_data data;

	ft_memset(&data, 0, sizeof(t_data));
	char	*input;

	while (1)
	{
		input = readline("\033[0;32mminishell$\033[0m ");
		data.input = input;
		if (!input)
			break ;
		parsing(input, envp, &data);
		add_history(input);
		free(input);								
	}
	return (0);
}