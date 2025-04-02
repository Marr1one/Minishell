/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 17:33:34 by maissat           #+#    #+#             */
/*   Updated: 2025/04/02 16:44:40 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data *g_data_child = NULL;

void	sigint_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	if (rl_done == 0)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();	
	}
}

void reset_signals_for_child(void)
{
    signal(SIGINT, SIG_DFL);  // Comportement par défaut pour SIGINT
    signal(SIGQUIT, SIG_DFL); // Comportement par défaut pour SIGQUIT

}

void	child_signal_handler(int signum)
{
	if (g_data_child)
		free_all(g_data_child->gc);
	exit(128 + signum);
}

void	setup_child_signals(t_data *data)
{
	g_data_child = data;
	signal(SIGINT, child_signal_handler);
	signal(SIGQUIT, child_signal_handler);
}