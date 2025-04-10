/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:16:02 by braugust          #+#    #+#             */
/*   Updated: 2025/04/08 04:24:40 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

void	setup_signals(void)
{
	rl_event_hook = simplefn;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_heredoc(void)
{
	rl_event_hook = simplefn;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
