/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:22:35 by maissat           #+#    #+#             */
/*   Updated: 2025/03/04 17:00:21 by maissat          ###   ########.fr       */
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
//typedef struct s_env
//{
//	char			*name;
//	char			*value;
//	struct s_env	*next;
//}	t_env;

typedef struct s_pipex_pipe
{
    int    read;
    int    write;
}    t_pipex_pipe;

typedef struct s_malloc_node
{
	void	*addr;
	struct s_malloc_node *next;
	
}	t_malloc_node;

typedef struct s_malloc
{
	t_malloc_node *first;	
}	t_malloc;

typedef enum e_type
{
	WORD,
	REDIRECTION,
	HEREDOC
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
	t_malloc	*gc;
	//t_env	*env_var;
	char	**envp;
	char 	**path;
	int		pipe[2];
	char	*input;
	char	*command_path;
	char	**args;
	char	**skipped_qu;
	char	**cmd_args;
	int		quotes;
	int		exit_status;
	
}	t_data;



void	parsing(char *input, char **env, t_data *data);
char	**copy_env(char **envp);
char	**ft_split(char *str, char c);
int		ft_strlen(const char *str);
void	*ft_memset(void *b, int c, size_t len);
char	*ft_join(char *str, char *add);
// int 	handle_env(t_data *data, char *str);
void 	ft_echo(t_data data);
void	check_exit_status(t_data *data);
char	*get_path_env(char **envp);
t_malloc	**get_gc(void);
char	**add_slash_all(char **tab);
void	show_malloc_list(t_malloc *list);
char	*add_slash(char	*str);
void	*ft_malloc(size_t size);
int		ft_strncmp(char *s1, char *s2, int n);
int		check_builtin(t_data *data);
void	ft_redirect(t_data *data, int i);
void		check_redirect(t_data *data);
int		case_redirection(t_data *data);
int		test_commands(t_data *data);
int		count_global_quotes(char *str);
char	*take_after(char *str, char c);
int		ft_strncmp(char *s1, char *s2, int n);
char	*ft_itoa(int n);
void	*ft_malloc(size_t size);
void	ft_cd(t_data *data);
int		ft_strcmp(char *s1, char *s2);
int		ft_strlcmp(char *s1, char *s2);
int		return_exit_status(t_data *data);
void	sigint_handler(int signum);
int		check_unclosed(t_data *data);
void	rm_qts_nodes(t_data *data);
int		is_alpha(char c);
// t_token	*findlast(t_token	*list);
void	show_tab(char **tab);
t_token	*add_chained_list(t_data *data);
void	show_list(t_token *list);
char	*ft_joinchar(char *str, char c);
char	*ft_strdup(char	*str);
int	check_dollar(t_data *data);
int		check_export_compatibility(char *str);
int		get_nbr_node(t_token *list);
int		ft_atoi(char	*str);
int		count_word2(char *str);
int		count_args(char **args);
void	destroy_node_quotes(t_data *data);
char	**skip_quotes(t_data *data);
void	check_exit_status(t_data *data);
int		return_exit_status(t_data *data);
void	ft_exit(t_data *data);
int	is_numeric(char	*str);
void	free_all(t_malloc *gc);
char *ft_strchr(const char *s, int c);
char *get_cmd_path(const char *cmd, char **envp);
t_pipex_pipe *init_pipes(int nb_cmd);
void 	try_dup2(int oldfd, int newfd);
void 	child_redirection(int pos, int nb_cmd, t_pipex_pipe *pipes);
void 	close_all_pipes(int nb_cmd, t_pipex_pipe *pipes);
void 	execute_pipex(t_data *data);
void 	execute_simple_command(t_data *data);
int 	count_tab(char **tab);
int 	heredoc_input(char *delimiter);
char 	*ft_strstr(const char *haystack, const char *needle);
char	**list_to_args(t_data *data);
char	**cut_last(char **tab, int	i);
int		list_len(t_token *list);
#endif