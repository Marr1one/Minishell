/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:16:21 by maissat           #+#    #+#             */
/*   Updated: 2025/04/02 22:51:39 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirect(t_data *data, int	i, char **envp)
{
	int fd;
	pid_t pid;

	data->path = ft_split(get_path_env(envp), ':');
	data->path = add_slash_all(data->path);
	if (test_commands(data) == 0)
	{
			pid = fork();
			if (pid == 0)
			{
				fd = open(data->args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd == -1)
				{
					perror("open");
					exit(1);
				}
				dup2(fd, STDOUT_FILENO);
				close(fd);
				execve(data->command_path, data->cmd_args, envp);
				perror("execve");
				exit(1);
			}
			waitpid(pid, NULL, 0);
		}
}

char	**create_cmd_args(t_data *data, int i)
{
	int	j;
	char **res;

	res = ft_malloc(i * sizeof(char *));
	j = 0;
	while (j < i)
	{
		res[j] = data->args[j];
		j++;
	}
	res[j] = NULL;
	return (res);
}

void	handle_append(t_data *data, int i)
{
	int fd;
	pid_t pid;

	data->path = ft_split(get_path_env(data->envp), ':');
	data->path = add_slash_all(data->path);
	if (test_commands(data) == 0)
	{
		pid = fork();
		if (pid == 0)
		{
			fd = open(data->args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror("open");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			execve(data->command_path, data->cmd_args, data->envp);
			perror("execve");
			exit(1);
		}
		waitpid(pid, NULL, 0);	
	}
}

void		handle_back(t_data *data, int i)
{
	int fd;
	pid_t pid;

	data->path = ft_split(get_path_env(data->envp), ':');
	data->path = add_slash_all(data->path);
	if (test_commands(data) == 0)
	{
		pid = fork();
		if (pid == 0)
		{
			fd = open(data->args[i + 1], O_RDONLY);
			if (fd == -1)
			{
				perror("open");
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			execve(data->command_path, data->cmd_args, data->envp);
			perror("execve");
			exit(1);
		}
		waitpid(pid, NULL, 0);	
	}
}


// on va verifier si on une redirect, laquelle et on redirige dans la bonne fonction
// toutes les fonctions de handle font la meme chose, il font comme pipex, il redirige la sortie de stdout vers ce quil ya apres la redirection
// note : ls > out out2 doit marcher, actuellement on peut juste rediriger vers un fichier!
int	case_redirection(t_data *data)
{
	t_token	*list;
	t_token *first_redir;
	int		input_fd;
	int 	status;
	int		output_fd;
	int		has_redir;
	pid_t	pid;
	
	input_fd = -1;
	output_fd = -1;
	has_redir = 0;
	first_redir = NULL;
	list = data->list;
    while (list != NULL) 
	{
        if (ft_strlcmp(list->content, "<") == 0) 
		{
            if (!list->next || !(*list->next->content)) 
			{
                printf("minishell: syntax error near unexpected token `newline'\n");
                return (0);
            }
            if (has_redir == 0) 
			{
                first_redir = list;
                has_redir = 1;
            }
            if (input_fd != -1)
                close(input_fd);
            input_fd = open(list->next->content, O_RDONLY);
            if (input_fd == -1) 
			{
                perror("open");
                if (output_fd != -1)
                    close(output_fd);
                return (0);
            }
        }
		else if (ft_strlcmp(list->content, "<<") == 0)
        {
            if (!list->next || !(*list->next->content))
            {
                printf("minishell: syntax error near unexpected token `newline'\n");
                if (input_fd != -1)
                    close(input_fd);
                if (output_fd != -1)
                    close(output_fd);
                return (0);
            }
            if (has_redir == 0) 
            {
                first_redir = list;
                has_redir = 1;
            }
            if (input_fd != -1)
                close(input_fd);
            if (input_fd == -1)
            {
                if (output_fd != -1)
                    close(output_fd);
                return (0);
            }
        }
        else if (ft_strlcmp(list->content, ">") == 0) 
		{
            if (!list->next || !(*list->next->content)) 
			{
                printf("minishell: syntax error near unexpected token `newline'\n");
                if (input_fd != -1)
                    close(input_fd);
                if (output_fd != -1)
                    close(output_fd);
                return (0);
            }
            if (has_redir == 0) {
                first_redir = list;
                has_redir = 1;
            }
            if (output_fd != -1)
                close(output_fd);
            output_fd = open(list->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd == -1) 
			{
                perror("open");
                if (input_fd != -1)
                    close(input_fd);
                return (0);
            }
        }
        else if (ft_strlcmp(list->content, ">>") == 0) 
		{
            if (!list->next || !(*list->next->content))
			{
                printf("minishell: syntax error near unexpected token `newline'\n");
                if (input_fd != -1)
                    close(input_fd);
                if (output_fd != -1)
                    close(output_fd);
                return (0);
            }
            if (has_redir == 0) 
			{
                first_redir = list;
                has_redir = 1;
            }
            if (output_fd != -1)
                close(output_fd);
            output_fd = open(list->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (output_fd == -1) 
			{
                perror("open");
                if (input_fd != -1)
                    close(input_fd);
                return (0);
            }
        }
        list = list->next;
    }
    if (has_redir == 0)
        return (1);
    data->cmd_args = create_cmd_args(data, first_redir->index);
    data->path = ft_split(get_path_env(data->envp), ':');
    data->path = add_slash_all(data->path);
    if (test_commands(data) == 0) 
	{
        pid = fork();
        if (pid == 0) 
		{
            if (input_fd != -1) 
			{
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            
            if (output_fd != -1) 
			{
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }
            execve(data->command_path, data->cmd_args, data->envp);
            perror("execve");
            exit(1);
        }
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            data->exit_status = WEXITSTATUS(status);
    }
    if (input_fd != -1)
        close(input_fd);
    if (output_fd != -1)
        close(output_fd);
    
    return (0);
}

int	only_space(char *input)
{
	int	i;

	i = 0;	
	while (input[i])
	{
		if (input[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}


int	test_commands2(t_data *data, char *str)
{
	int	i;
	char *path_test;
	
	i = 0;
	if (only_space(data->input) == 0)
		return (1);
	if (data->path == NULL)
		return (1);
	while (data->path[i])
	{
		path_test = ft_join(data->path[i], str);
		if (access(path_test, F_OK | X_OK) == 0)
		{
			data->command_path = ft_strdup(path_test);
			// free(path_test);
			return (0);
		}
		i++;
	}
	// free(path_test);
	return (1);
}

int	ft_empty(t_data *data)
{
	if (data->list->content[0] == '\0')
		return (1);
	return (0);
}


int		check_empty(t_data data)
{
	int	i;
	int	len_tab;
	char	**tab;
	
	i = 0;
	len_tab = 0;
	tab = data.args;
	while (tab[len_tab])
		len_tab++;	
	if (!data.input)
		return (1);
	while(data.input[i] == ' ')
		i++;
	if (data.input[i] == '\0')
		return (1);
	if (len_tab == 1 && data.args[0][0] == '\0')
		return (1);
	return (0);
}
char	*take_before(char *str, char c)
{
	char	*res;
	int		i;
	int		len;
	
	i = 0;
	while(str[i] != c)
		i++;
	len = i;
	res = ft_malloc(sizeof(char) * (len + 1));
	i = 0;
	while (i < len)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
	
}

void	ft_redirect(t_data *data, int i)
{
	(void)i;
	pid_t	pid;
	int		fd;
	char	*before;

	before = take_before(data->input, '>');
	if (test_commands2(data, before) == 0)
	{
		pid = fork();
		if (pid == 0)
		{
			fd = open(take_after(data->input, '>'), O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("open");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			execve(data->command_path, data->cmd_args, data->envp);
			perror("execve");
			exit(1);
		}
		waitpid(pid, NULL, 0);
	}
}

int	count_word2(char *str)
{
	int	i;
	int	count;
	int	in_word;
	
	i = 0;
	in_word = 0;
	count = 0;
	while (str[i])
	{
		if(str[i] != '>')
		{
			if (in_word == 0)
			{
			in_word = 1;
			count++;
			}
			
		}
		else
		{
			count++;
			in_word = 0;
		}
		i++;
	}
	return (count);
}

int	count_redirect(t_data *data)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (data->input[i])
	{
		if (data->input[i] == '>')
			count++;
		i++;
	}
	return (count);
}

void	ft_clean_input(t_data *data, char c)
{
	char	*new_input;
	int		i;
	int		j;
	
	i = 0;
	j = 0;
	new_input = ft_malloc(ft_strlen(data->input) + (2 * count_redirect(data) + 1));
	if (!new_input)
		return ;
	while (data->input[i])
	{
		if (data->input[i] == c)
		{
			if (i > 0 && data->input[i - 1] != ' ')
			{
				new_input[j] = ' ';
				j++;
			}
			new_input[j] = c;
			j++;
			if (data->input[i + 1] == c)
			{
				i++;
				new_input[j] = c;
				j++;
			}
			if (data->input[i + 1] && data->input[i + 1] != ' ')
			{
				new_input[j] = ' ';
				j++;
			}
		}
		else
		{
			new_input[j] = data->input[i];
			j++;
		}
		i++;
	}
	new_input[j] = '\0';
	// free(data->input);
	data->input = new_input;
}



void	check_redirect(t_data *data)
{
	int		i;
	// int		j;
	// int		len;
	//char	**tab;

	// len = count_word2(data->input);
	//tab = malloc(sizeof(char *) * (len + 1));
	i = 0;
	// j = 0;
	while (data->input[i])
	{
		if(data->input[i] == '>')
			ft_clean_input(data, '>');
		i++;
	}
}



//char	*delete_quotes_str(char *str)
//{
//	int	i;
//	int	j;
//	int	count;
//	char	*res;

//	count = 0;
//	i = 0;
//	j = 0;
//	while (str[i])
//	{
//		if (str[i] == '"')
//		{
//			count++;
//		}
//		i++;
//	}
//	res = malloc(sizeof(char) * (i - count + 1));
//	i = 0;
//	j = 0;
//	while (str[i])
//	{
//		if (str[i] != '"')
//		{
//			res[j] = str[i];
//			i++;
//			j++;			
//		}
//		else
//		{
//			i++;
//		}
//	}
//	res[j] = '\0';
//	return (res);
//}

// void	delete_quotes_hard(t_data *data)
// {
// 	t_token	*list;

// 	list = data->list;
// 	while (list != NULL)
// 	{
// 		list->content = ft_strdup(delete_quotes_str(list->content));
// 		list = list->next;
// 	}
// }

// void	delete_quotes_inside(t_data *data)
// {
// 	t_token	*list;
// 	int		i;

// 	i = 0;
// 	list = data->list;
// 	while (list != NULL)
// 	{
// 		i = 0;
// 		while (list->content[i] == '"')
// 			i++;
// 		if (list->content[i] != '\0')
// 		{
// 			//printf("%s\n", list->content);
// 			list->content = ft_strdup(delete_quotes_str(list->content));
// 		}
// 		list = list->next;
// 	}
// }


//|| (input[0] == '.' && input[1] == '/'))

