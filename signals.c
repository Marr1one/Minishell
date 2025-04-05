/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 17:33:34 by maissat           #+#    #+#             */
/*   Updated: 2025/04/05 15:20:29 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	variable_globale = 0;

t_data	*get_gdata(void)
{
	static	t_data g_data_child = {0};

	return (&g_data_child);
}

void	sigint_handler_heredoc(int signum)
{
	variable_globale = signum;
	rl_done = 1;
}

void	sigint_handler(int signum)
{
	variable_globale = signum;
	rl_done = 1;
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
int	simplefn(void)
{
	return (0);
}
