/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:27:02 by maissat           #+#    #+#             */
/*   Updated: 2025/04/07 15:54:40 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_data(t_data *data, char **envp)
{
	ft_memset(data, 0, sizeof(*data));
	data->envp = copy_env(envp);
	data->gc = get_gc();
}

int	check_arguments(int argc)
{
	if (argc != 1)
	{
		printf("Usage : ./minishell\n");
		return (0);
	}
	return (1);
}

void	process_command(char *input, t_data *data)
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
	expand_all(list_cmd, data);
	remove_quotes(list_cmd);
	execute_cmds(data, list_cmd);
}

/* Boucle principale du shell */
void	shell_loop(t_data *data)
{
	char	*input;	

	while (1)
	{
		if (variable_globale != 0)
		{
			data->exit_status = 128 + variable_globale;
			variable_globale = 0;
		}
		
		input = readline("\033[0;32mminishell\033[0m ");
		if (!input)
		{
			free_all(data->gc);
			free(input);
			break ;
		}
		if (variable_globale != 0)
			continue;
		// Move the variable_globale check to before reading input
		// so it's already reset when we process the next command
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
		return(printf("minishell: no tty\n"), 1);
	(void)argv;
	if (!check_arguments(argc))
		return (1);
	initialize_data(&data, envp);
	setup_signals();
	shell_loop(&data);
	return (0);
}
