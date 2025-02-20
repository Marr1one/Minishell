/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:16:21 by maissat           #+#    #+#             */
/*   Updated: 2025/02/20 15:42:32 by maissat          ###   ########.fr       */
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

	res = malloc(i * sizeof(char *));
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

int	case_redirection(t_data *data, char **envp)
{
	t_token	*list;
	
	list = data->list;
	while (list != NULL)
	{
		if (ft_strlcmp(list->content, ">") == 0)
		{
			//printf("on trouve >\n");
			if (!list->next || !(*list->next->content))
			{
				printf("Nul part ou rediriger!\n");
				return (0);
			}
			data->cmd_args = create_cmd_args(data, list->index);
			handle_redirect(data, list->index, envp);
			return (0);	
		}
		else if (ft_strlcmp(list->content, "<") == 0)
		{
			//printf("in < case\n");
			//printf("on trouve >\n");
			if (!list->next || !(*list->next->content))
			{
				printf(" Rien apres le <!\n");
				return (0);
			}
			data->cmd_args = create_cmd_args(data, list->index);
			handle_back(data, list->index);
			//printf("ICI!\n");
			return (0);	
		}
		else if (ft_strlcmp(list->content, ">>") == 0)
		{
			//printf("in < case\n");
			//printf("on trouve >\n");
			if (!list->next || !(*list->next->content))
			{
				printf(" Rien apres le >>!\n");
				return (0);
			}
			data->cmd_args = create_cmd_args(data, list->index);
			handle_append(data, list->index);
			//printf("ICI!\n");
			return (0);	
		}
		list = list->next;
	}
	return (1);
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
			free(path_test);
			return (0);
		}
		i++;
	}
	free(path_test);
	return (1);
}

int	ft_empty(t_data *data)
{
	if (data->list->content[0] == '\0')
		return (1);
	return (0);
}

int	test_commands(t_data *data)
{
	printf("in test command!\n");
	int	i;
	char *path_test;
	
	i = 0;
	if (ft_empty(data) == 1)
	{
		return (1);
	}
	if (only_space(data->input) == 0)
		return (1);
	if (data->path == NULL)
		return (1);
	while (data->path[i])
	{
		path_test = ft_join(data->path[i], data->list->content);
		if (access(path_test, F_OK | X_OK) == 0)
		{
			data->command_path = ft_strdup(path_test);
			free(path_test);
			return (0);
		}
		i++;
	}
	free(path_test);
	return (1);
}

int		check_empty(t_data data)
{
	if (!data.list)
		return (1);
	if (data.list->content[0] == '\0')
	{
		return (1);
	}
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
	res = malloc(sizeof(char) * (len + 1));
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

void	ft_clean_input(t_data *data)
{
	char	*new_input;
	int		i;
	int		j;
	
	i = 0;
	j = 0;
	new_input = malloc(ft_strlen(data->input) + (2 * count_redirect(data) + 1));
	if (!new_input)
		return ;
	while (data->input[i])
	{
		if (data->input[i] == '>')
		{
			if (i > 0 && data->input[i - 1] != ' ')
			{
				new_input[j] = ' ';
				j++;
			}
			new_input[j] = '>';
			j++;
			if (data->input[i + 1] == '>')
			{
				i++;
				new_input[j] = '>';
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
	free(data->input);
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
		{
			ft_clean_input(data);
		}
		i++;
	}
}

void	exec_command(t_data *data)
{
	//printf("dans execcommand!\n");
	pid_t	pid;
	int		status;
	
	pid = fork();
	if (pid == 0)
	{
		//data->exit_status = 0;
		signal(SIGINT, SIG_DFL);
		execve(data->command_path, data->args, data->envp);
		perror("execve");
		//check_exit_status(data);
		exit(127);
	}	
	//signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		printf("in signaled !\n");
		int signal_number = WTERMSIG(status);
		printf("Process terminated by signal: %d\n", signal_number);
		data->exit_status = 128 + signal_number;
		printf("data.exitstatus after signal ; %d\n", data->exit_status);
		//if (WTERMSIG(status) == SIGINT)
		//    data->exit_status = 130;
	}
	//signal(SIGINT, sigint_handler);
	else if (WIFEXITED(status))
	{
		data->exit_status = WEXITSTATUS(status);
		//check_exit_status(data);
	}
	else
	{
		data->exit_status = 128;
		//check_exit_status(data);
	}
	return;
}

char	*delete_quotes_str(char *str)
{
	int	i;
	int	j;
	int	count;
	char	*res;

	count = 0;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"')
		{
			count++;
		}
		i++;
	}
	res = malloc(sizeof(char) * (i - count + 1));
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '"')
		{
			res[j] = str[i];
			i++;
			j++;			
		}
		else
		{
			i++;
		}
	}
	res[j] = '\0';
	return (res);
}

void	delete_quotes_hard(t_data *data)
{
	t_token	*list;

	list = data->list;
	while (list != NULL)
	{
		list->content = ft_strdup(delete_quotes_str(list->content));
		list = list->next;
	}
}

void	delete_quotes_inside(t_data *data)
{
	t_token	*list;
	int		i;

	i = 0;
	list = data->list;
	while (list != NULL)
	{
		i = 0;
		while (list->content[i] == '"')
			i++;
		if (list->content[i] != '\0')
		{
			printf("dans ce cas la\n");
			printf("%s\n", list->content);
			list->content = ft_strdup(delete_quotes_str(list->content));
		}
		list = list->next;
	}
}

void	parsing(char *input, char **envp, t_data *data)
{
	pid_t	pid;
	int		status;
	
	//printf("in parsing\n");
	if (count_global_quotes(data) % 2 != 0)
	{
		data->exit_status = 127;
		printf("nombre de guillemet : %d\n", count_global_quotes(data));
		printf("minishell: unclosed quote detected\n");
		return ;
	}
	
	delete_quotes_inside(data);
	printf("apres delete quotes\n");
	show_list(data->list);
	//idee ; si cest pair, aller dans chaque noeud et enlever toutes les guillemets!

	
	//if (check_unclosed(data) == 1)
	//{
	//	data->exit_status = 127;
	//	printf("minishell: unclosed quote detected\n");
	//	return ;
	//}
	//remove_quotes_all(data);
	//show_list(data->list);
	if (check_empty(*data) == 1)
	{
		printf("in empty\n");
		//printf("minishell: %s: command not found\n", data->args[0]);
		if (data->quotes == 1)
		{
			data->exit_status = 127;
			printf("minishell: : command not found\n");
		}
		return ;
	}
	//if (return_exit_status(data) == 1)
	//{
	//	printf("minishell: %s: command not found\n", data->list->content);
	//	return;
	//}
	//printf("after remove\n");
	//show_tab(data->args);
	if (check_builtin(data) != 0)
		return;
	delete_quotes_hard(data);
	printf("apres delete quotes hard\n");
	show_list(data->list);
	if (input[0] == '/' || (input[0] == '.' && input[1] == '/'))
	{
		if (access(input, F_OK) != 0)
		{
            printf("minishell: %s: No such file or directory\n", input);
            return;
        }
        if (access(input, X_OK) != 0)
        {
			data->exit_status = 126;
            printf("minishell: %s: Permission denied\n", input);
            return;
        }
		pid = fork();
		if (pid == 0)
		{
			execve(input, data->args, envp);
			perror("execve");
			//data->exit_status = 127;
			//check_exit_status(data);
			exit(127);
		}
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
        {
			printf("in signaled !\n");
            data->exit_status = 128 + WTERMSIG(status);
        }
		if (WIFEXITED(status))
		{
			printf("in wifexited !\n");
			data->exit_status = WEXITSTATUS(status);
			//check_exit_status(data);
		}
		else
		{
			printf("in else case of sign\n");
        	data->exit_status = 128;
			//check_exit_status(data);
		}
		return;
	}
	data->path = ft_split(get_path_env(envp), ':');
	data->path = add_slash_all(data->path);
	//if (check_redirect(data) != 0)
	//	return ;
	if (test_commands(data) == 0)
	{
		exec_command(data);
		return;
	}
	else
	{
		printf("dans le else\n");
		data->exit_status = 127;
		//check_exit_status(data);
		printf("minishell: %s: command not found\n", data->list->content);
	}
	//if (only_space(data->input) == 0)
	//	return ;
	
}
