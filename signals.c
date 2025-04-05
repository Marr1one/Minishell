/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 17:33:34 by maissat           #+#    #+#             */
/*   Updated: 2025/04/05 13:31:43 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	variable_globale = 0;

t_data *get_gdata(void)
{
	static t_data g_data_child = {0}; // Initialisé à 0

	return (&g_data_child);
}

void sigint_handler_heredoc(int signum)
{
	variable_globale = signum;
    rl_done = 1;                  // Indique à readline qu'il faut arrêter
}

void sigint_handler(int signum)
{
	variable_globale = signum;
    rl_done = 1;
}


//void	child_signal_handler(int signum)
//{
//	if (g_data_child)
//		free_all(g_data_child->gc);
//	exit(128 + signum);
//}

void	setup_child_signals()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
int		simplefn()
{
	return (0);
}

void	setup_signals(void)
{
	rl_event_hook = simplefn;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_heredoc(void)
{
	rl_event_hook = simplefn;
	signal(SIGINT, sigint_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
