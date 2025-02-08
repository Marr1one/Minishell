/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:22:35 by maissat           #+#    #+#             */
/*   Updated: 2025/02/08 20:00:21 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	MINISHELL_H

#define	MINISHELL_H

#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


//IDEE = si on a un content avec genre >out, le coupe en deux, enfin isoler dans cas le >
typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum e_type
{
	WORD,
	REDIRECTION,
}   t_type;

typedef struct s_token
{
	t_type			type;
	char			*content;
	int				index;
	struct	s_token	*next;
}	t_token;

typedef struct s_data
{
	t_token	*list;
	t_env	*env_var;
	char	**envp;
	char 	**path;
	int		pipe[2];
	char	*input;
	char	*command_path;
	char	**args;
	char	**cmd_args;
}	t_data;

void	parsing(char *input, char **env, t_data *data);
char	**copy_env(char **envp);
char	**ft_split(char *str, char c);
int		ft_strlen(char *str);
void	*ft_memset(void *b, int c, size_t len);
char	*ft_join(char *str, char *add);
// int 	handle_env(t_data *data, char *str);
void 	ft_echo(t_data data);
char	*get_path_env(char **envp);
char	**add_slash_all(char **tab);
char	*add_slash(char	*str);
int		ft_strncmp(char *s1, char *s2, int n);
int		check_builtin(t_data *data);
int		case_redirection(t_data *data, char **envp);
int		test_commands(t_data *data);
int		ft_strncmp(char *s1, char *s2, int n);
int		ft_strcmp(char *s1, char *s2);
int		ft_strlcmp(char *s1, char *s2);
void	sigint_handler(int signum);
void	remove_quotes_all(t_data *data);
int		is_alpha(char c);
// t_token	*findlast(t_token	*list);
void	show_tab(char **tab);
t_token	*add_chained_list(t_data *data);
char	*ft_joinegal(char *str);
char	*ft_strdup(char	*str);
void		check_dollar(t_data *data);
int		check_export_compatibility(char *str);
int		get_nbr_node(t_token *list);

#endif