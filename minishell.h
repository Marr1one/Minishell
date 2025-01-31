/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:22:35 by maissat           #+#    #+#             */
/*   Updated: 2025/01/29 17:34:18 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	MINISHELL_H

#define	MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct s_data
{
	char 	**path;
	char	*input;
	char	*command;
}	t_data;

void	parsing(char *input, char **env, t_data *data);
char	**ft_split(char *str, char c);
int		ft_strlen(char *str);
void	*ft_memset(void *b, int c, size_t len);
char	*ft_join(char *str, char *add);
char	*get_path_env(char **envp);
char	**add_slash_all(char **tab);
char	*add_slash(char	*str);

#endif