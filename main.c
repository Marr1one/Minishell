/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/04/09 16:27:38 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Boucle principale du shell */
void	shell_loop(t_data *data)
{
	char	*input;

	while (1)
	{
		if (g_signalhook != 0)
		{
			data->exit_status = 128 + g_signalhook;
			g_signalhook = 0;
		}
		input = readline(GREEN "minishell" RESET " ");
		if (!input)
		{
			free_all(data->gc);
			free(input);
			break ;
		}
		if (g_signalhook != 0)
			continue ;
		add_history(input);
		process_command(input, data);
		free(input);
	}
}

/* Fonction principale */
int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (isatty(STDIN_FILENO) == 0 || isatty(STDOUT_FILENO) == 0)
		return (printf("minishell: no tty\n"), 1);
	(void)argv;
	if (!check_arguments(argc))
		return (1);
	initialize_data(&data, envp);
	setup_signals();
	shell_loop(&data);
	return (0);
}
