/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:22:35 by maissat           #+#    #+#             */
/*   Updated: 2025/04/07 16:08:46 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

#define GREEN   "\001\033[0;32m\002"
#define RESET   "\001\033[0m\002"

typedef struct s_idx
{
	int						i;
	int						j;
}							t_idx;

typedef struct s_malloc_node
{
	void					*addr;
	struct s_malloc_node	*next;

}							t_malloc_node;

typedef struct s_malloc
{
	t_malloc_node			*first;
}							t_malloc;

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
}							t_type;

typedef struct s_token
{
	t_type					type;
	char					*content;
	int						index;
	struct s_token			*next;
}							t_token;

typedef struct s_file
{
	t_type					mode;
	char					*path;
	struct s_file			*next;
}							t_file;

typedef struct s_cmd
{
	char					**args;
	t_file					*files;
	char					*heredoc_content;
	struct s_cmd			*next;
}							t_cmd;

typedef struct s_data
{
	t_token					*list_tkn;
	t_type					expect;
	t_malloc				**gc;
	int						in_quote;
	char					**envp;
	char					**path;
	char					*input;
	char					*command_path;
	char					**args;
	char					**skipped_qu;
	char					**cmd_args;
	char					quote;
	int						quotes;
	t_token					*list;
	int						exit_status;

}							t_data;

extern int					variable_globale;

char						*init_heredoc_loop(void);
int							is_tkn_redir(t_token *token);
t_data						*get_gdata(void);
void						sigint_handler_heredoc(int signum);
void						sigint_handler(int signum);
void						setup_child_signals(void);
int							simplefn(void);
char						*quoteless_string(char *str);
void						setup_signals_heredoc(void);
int							print_error(char *message);
char						*get_target_dir(t_data *data);
char						*search_oldpwd(t_data *data);
void						update_oldpwd(t_data *data, char *pwd);
void						update_pwd(t_data *data, char *pwd);
void						expand_argument(char **args, int i, t_data *data);
void						expand_file_path(t_file *file, t_data *data);
void						expand_cmd_files(t_cmd *cmd, t_data *data);
char						*expand_exit_status(t_data *data);
char						**create_new_args(char **args, int idx);
int							open_file(char *path, t_type mode);
int							count_cmds(t_cmd *list_cmds);
int							execute_unset(t_cmd *cmd, t_data *data);
int							execute_simple_builtin(t_cmd *cmd, t_data *data);
int							ncheck_builtin(t_cmd *cmd);
int							execute_builtin_child(t_cmd *cmd, t_data *data);
char						*new_test_commands(char **paths, char *str);
int							test_relative_path(char *path_test);
int							handle_file_redirections(t_cmd *current_cmd);
void						execute_child_process(t_data *data, t_cmd *cmd,
								int fd_in, int *fd_pipe);
int							handle_all_heredocs(t_data *data, t_cmd *cmds);
int							check_single_builtin(t_data *data, t_cmd *cmds);
void						setup_pipe(t_cmd *cmd, int *fd_pipe);
void						execute_forked_cmd(t_data *data, t_cmd *cmd,
								int *fd_in, int *fd_pipe);
void						redirect_pipes(t_cmd *current_cmd, int fd_in,
								int *fd_pipe);
void						setup_heredoc(t_cmd *cmd);
void						handle_heredoc_and_files(t_cmd *cmd);
void						execute_command(t_data *data, t_cmd *cmd);
int							redirect_fd(int fd, int mode);
int							handle_single_file(t_file *file);
int							execute_unset(t_cmd *cmd, t_data *data);
int							execute_simple_builtin(t_cmd *cmd, t_data *data);
int							execute_advanced_builtin(t_cmd *cmd, t_data *data);
int							check_len(char *s1, char *s2);
int							countword(char *str, char c);
char						*custom_worddup(char *str, int start, int end);
char						*worddup(char *str, int start, int end);
t_file						*find_last_node(t_file *node);
t_file						*find_existing_heredoc(t_file *node);
char						*read_heredoc(t_data *data,
								char *delimiter, char *prompt);
void						child_signal_handler(int signum);
void						setup_child_signals(void);
void						parent_signal_handler(int sig);
void						setup_parent_signal_handlers(void);
void						sigint_handler(int signum);
void						child_signal_handler(int sig);
void						reset_signals_for_child(void);
char						*check_env(t_data *data, char *str);
int							check_variable_in_env(char *var_name, t_data *data);
void						handle_heredoc(t_data *data, t_cmd *current_cmd);
void						expand_all(t_cmd *cmd, t_data *data);
int							handle_quotes(char c, t_data *data);
int							is_redirect(char c);
int							is_pipe(char c);
int							is_space(char c);
int							len_exit_status(t_data *data);
int							len_var_value(const char *arg, int *i,
								t_data *data);
int							handle_dollar_len(const char *arg, int *i,
								t_data *data);
int							calc_final_len(const char *arg, t_data *data);
int							append_var_value(char *result, int *j,
								char *var_name, t_data *data);
int							handle_dollar(char *result, char *arg, t_idx *idx,
								t_data *data);
int							build_final_string(char *result, char *arg,
								t_data *data);
int							get_dollar_count(const char *arg, int *i);
char						*extract_var_name(const char *arg, int *i);
char						*expand_string(char *arg, t_data *data);
char						*ft_substr(char *str, int start, int end);
void						show_list_cmd(t_cmd *list);
void						show_tab(char **tab);
void						show_list(t_token *list);
void						show_malloc_list(t_malloc *list);
t_token						*findlast_token(t_token *list);
int							is_digit(char c);
int							is_redirect(char c);
int							is_pipe(char c);
int							is_space(char c);
t_token						*tokenizer(char *input, t_data *data);
const char					*get_token_type_name(t_type type);
// void			execute_cmds(t_cmd *cmds, char **paths);
void						execute_cmds(t_data *data, t_cmd *cmds);
// int 			execute_builtin(t_cmd *cmd, t_data *data);
int							show_env(char **tab);
char						*ft_substr_qte(char *str, int start, int end);
char						**ft_unset(t_data *data, int save);
void						check_unset(t_data *data, char *str);
int							is_alphanum(char c);
void						reset_signals_for_child(void);
t_cmd						*parse_cmd(t_token *list);
void						remove_quotes(t_cmd *list_cmd);
t_cmd						*create_files(t_token *list_tkn, t_cmd *list_cmd);
t_cmd						*create_args(t_token *lst_tkn, t_cmd *lst_cmd);
void						setup_signals(void);
t_type						save_mode(t_token current_tkn);
void						add_arg(t_token *curr_tkn, t_cmd *curr_cmd, int *i);
t_file						*add_file(t_file *list_file, t_token *file_tkn,
								t_type mode);
int							count_arguments(t_token *list);

// void			parsing(char **envp, t_data *data);
char						**copy_env(char **envp);
int							test_commands(t_data *data);
char						*take_before(char *str, char c);
void						exec_command(t_data *data);
char						**ft_split(char *str, char c);
int							ft_strlen(const char *str);
void						*ft_memset(void *b, int c, size_t len);
char						*ft_join(char *str, char *add);
// int 	handle_env(t_data *data, char *str);
void						ft_clean_input(t_data *data, char c);
void						ft_echo(t_cmd *current_cmd);
// void			check_exit_status(t_data *data);
char						*get_path_env(char **envp);
t_malloc					**get_gc(void);
char						**add_slash_all(char **tab);
void						show_malloc_list(t_malloc *list);
char						*add_slash(char *str);
void						*ft_malloc(size_t size);
int							ft_strncmp(char *s1, char *s2, int n);
int							check_builtin(t_data *data);
void						ft_redirect(t_data *data, int i);
int							ft_empty(t_data *data);
void						check_redirect(t_data *data);
int							case_redirection(t_data *data);
int							test_commands(t_data *data);
int							count_global_quotes(char *str);
char						*take_after(char *str, char c);
int							ft_strncmp(char *s1, char *s2, int n);
char						*ft_itoa(int n);
void						*ft_malloc(size_t size);
int							ft_cd(t_cmd *cmd, t_data *data);
int							ft_strcmp(char *s1, char *s2);
int							ft_strlcmp(char *s1, char *s2);
// int				return_exit_status(t_data *data);
void						sigint_handler(int signum);
// int				check_unclosed(t_data *data);
void						rm_qts_nodes(t_data *data);
int							is_alpha(char c);
// t_token	*findlast(t_token	*list);
void						show_tab(char **tab);
t_token						*add_chained_list(char **tab);
void						show_list(t_token *list);
char						*ft_joinchar(char *str, char c, int x);
char						*ft_strdup(char *str);
// int				check_dollar(t_data *data);
int							check_export_compatibility(char *str);
int							get_nbr_node(t_token *list);
int							ft_atoi(char *str);
int							count_word2(char *str);
int							count_args(char **args);
void						destroy_node_quotes(t_data *data);
char						**skip_quotes(t_data *data);
// void			check_exit_status(t_data *data);
// int				return_exit_status(t_data *data);
void						ft_exit(t_cmd *cmd, t_data *data);
int							is_numeric(char *str);
void						free_all(t_malloc **gc);
void						destroy_empty_node(t_data *data);
char						**custom_split(char *str, char c);
// pipex
int							char_in_string(char *str, char c);
char						*get_cmd_path(const char *cmd, char **envp);
int							count_tab(char **tab);
int							is_builtin(char *cmd);
// void			execute_builtin(t_data *data, char **args);
// heredoc
char						*execute_heredocs(t_data *data, t_file *files,
								int last_index);
char						*process_heredoc(t_data *data, t_file *file,
								int current_index, int last_index);
int							contains_heredoc(t_cmd *cmd);
char						*execute_last_heredoc(t_data *data, t_cmd *cmd);
t_file						*find_last_heredoc(t_file *files, int *last_index);
int							heredoc_input(char *delimiter);
char						*ft_strstr(const char *haystack,
								const char *needle);
char						**list_to_args(t_data *data);
char						**cut_last(char **tab, int i);
int							list_len(t_token *list);
int							only_space(char *input);
int							ft_pwd(t_cmd *cmd);
int							ft_export(t_cmd *cmd, t_data *data);
char						**add_export(t_data *data, char *str);
void						show_tab_export(char **tab);
int							check_change(t_data *data, char *str);

int							validate_input(const char *input);
char						*ft_strjoin(char const *s1, char const *s2);
char						*ft_re_strjoin(char *s1, char const *s2);
char						*ft_strcpy(char *dest, char *src);
char						*ft_strcat(char *dest, char *src);

#endif