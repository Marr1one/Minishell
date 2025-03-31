/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:22:35 by maissat           #+#    #+#             */
/*   Updated: 2025/03/31 20:08:42 by braugust         ###   ########.fr       */
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

typedef struct s_idx
{
	int	i;
	int	j;
}	t_idx;

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
	CMD,
	ARG,
	INFILE,
	OUTFILE_APPEND,
	OUTFILE_TRUNC,
	HEREDOC,
	PIPE,
	FICHIER,
	UNKNOWN,
}   t_type;

typedef struct s_token
{
	t_type			type;
	char			*content;
	int				index;
	struct	s_token	*next;
}	t_token;

typedef struct s_file
{
	t_type mode;
	char *path;
	struct s_file *next;
} t_file;

typedef struct s_cmd
{
	char **args;
	t_file *files;	
	struct	s_cmd	*next;
}	t_cmd;

typedef struct s_data
{
	t_token		*list_tkn;
	t_type		expect;
	t_malloc	**gc;
	//t_env	*env_var;
	int		in_quote;
	char	**envp;
	char 	**path;
	int		pipe[2];
	char	*input;
	char	*command_path;
	char	**args;
	char	**skipped_qu;
	char	**cmd_args;
	char     quote;
	int		quotes;
	t_token		*list;
	int		exit_status;
	
}	t_data;

char			*check_env(t_data *data, char *str);
int				check_variable_in_env(char *var_name, t_data *data);
void 			handle_heredoc(t_cmd *current_cmd);
void			expand_all(t_cmd *cmd, t_data *data);
int 			handle_quotes(char c, t_data *data);
int				is_redirect(char c);
int				is_pipe(char c);
int				is_space(char c);
 int			len_exit_status(t_data *data);
 int			len_var_value(const char *arg, int *i, t_data *data);
 int			handle_dollar_len(const char *arg, int *i, t_data *data);
int				calc_final_len(const char *arg, t_data *data);
int			append_var_value(char *result, int *j, char *var_name, t_data *data);
int			handle_dollar(char *result, const char *arg, t_idx *idx, t_data *data);
int			build_final_string(char *result, const char *arg, t_data *data);
int				get_dollar_count(const char *arg, int *i);
char			*extract_var_name(const char *arg, int *i);
char			*expand_string(const char *arg, t_data *data);
char			*ft_substr(char *str,  int start, int end);
void			show_list_cmd(t_cmd *list);
void			show_tab(char **tab);
void			show_list(t_token *list);
void			show_malloc_list(t_malloc *list);
t_token			*findlast_token(t_token	*list);
int				is_digit(char c);
int				is_redirect(char c);
int				is_pipe(char c);
int				is_space(char c);
t_token			*tokenizer(char *input, t_data *data);
const char		*get_token_type_name(t_type type);
// void			execute_cmds(t_cmd *cmds, char **paths);
void 			execute_cmds(t_data *data, t_cmd *cmds);
int 			execute_builtin(t_cmd *cmd, t_data *data);
int				show_env(char **tab);
char			*ft_substr_qte(char *str,  int start, int end);
char			**ft_unset(t_data *data, int	save);
void			check_unset(t_data *data, char	*str);
int				is_alphanum(char c);


// void			parsing(char **envp, t_data *data);
char			**copy_env(char **envp);
int				test_commands(t_data *data);
char			*take_before(char *str, char c);
void			exec_command(t_data *data);
char			**ft_split(char *str, char c);
int				ft_strlen(const char *str);
void			*ft_memset(void *b, int c, size_t len);
char			*ft_join(char *str, char *add);
// int 	handle_env(t_data *data, char *str);
void			ft_clean_input(t_data *data, char c);
void 			ft_echo(t_cmd *current_cmd);
void			check_exit_status(t_data *data);
char			*get_path_env(char **envp);
t_malloc		**get_gc(void);
char			**add_slash_all(char **tab);
void			show_malloc_list(t_malloc *list);
char			*add_slash(char	*str);
void			*ft_malloc(size_t size);
int				ft_strncmp(char *s1, char *s2, int n);
int				check_builtin(t_data *data);
void			ft_redirect(t_data *data, int i);
int				ft_empty(t_data *data);
void			check_redirect(t_data *data);
int				case_redirection(t_data *data);
int				test_commands(t_data *data);
int				count_global_quotes(char *str);
char			*take_after(char *str, char c);
int				ft_strncmp(char *s1, char *s2, int n);
char			*ft_itoa(int n);
void			*ft_malloc(size_t size);
int				ft_cd(t_cmd *cmd);
int				ft_strcmp(char *s1, char *s2);
int				ft_strlcmp(char *s1, char *s2);
int				return_exit_status(t_data *data);
void			sigint_handler(int signum);
int				check_unclosed(t_data *data);
void			rm_qts_nodes(t_data *data);
int				is_alpha(char c);
// t_token	*findlast(t_token	*list);
void			show_tab(char **tab);
t_token			*add_chained_list(char **tab);
void			show_list(t_token *list);
char			*ft_joinchar(char *str, char c, int x);
char			*ft_strdup(char	*str);
int				check_dollar(t_data *data);
int				check_export_compatibility(char *str);
int				get_nbr_node(t_token *list);
int				ft_atoi(char	*str);
int				count_word2(char *str);
int				count_args(char **args);
void			destroy_node_quotes(t_data *data);
char			**skip_quotes(t_data *data);
void			check_exit_status(t_data *data);
int				return_exit_status(t_data *data);
void			ft_exit(t_cmd *cmd, t_data *data);
int				is_numeric(char	*str);
void			free_all(t_malloc **gc);
void 			destroy_empty_node(t_data *data);
char			**custom_split(char *str, char c);
//pipex
char 			*ft_strchr(const char *s, int c);
char 			*get_cmd_path(const char *cmd, char **envp);
void 			try_dup2(int oldfd, int newfd);
void 			execute_pipex(t_data *data);
void 			execute_simple_command(t_data *data);
int 			count_tab(char **tab);
int				is_builtin(char *cmd);
// void			execute_builtin(t_data *data, char **args);
// heredoc
int				contains_heredoc(t_cmd *cmd);
char 			*execute_last_heredoc(t_cmd *cmd);
int 			heredoc_input(char *delimiter);
char 			*ft_strstr(const char *haystack, const char *needle);
char			**list_to_args(t_data *data);
char			**cut_last(char **tab, int	i);
int				list_len(t_token *list);
int				only_space(char *input);
int				ft_pwd(t_cmd *cmd);
int				ft_export(t_cmd *cmd, t_data *data);
char			**add_export(t_data *data, char *str);
void			show_tab_export(char **tab);
int				check_change(t_data *data, char *str);

int 	validate_input(const char *input);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_re_strjoin(char *s1, char const *s2);
char	*ft_strcpy(char *dest, char *src);
char	*ft_strcat(char *dest, char *src);

#endif