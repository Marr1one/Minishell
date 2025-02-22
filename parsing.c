/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:16:21 by maissat           #+#    #+#             */
/*   Updated: 2025/02/23 18:43:59 by maissat          ###   ########.fr       */
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
int	case_redirection(t_data *data, char **envp)
{
	t_token	*list;
	
	list = data->list;
	while (list != NULL)
	{
		if (ft_strlcmp(list->content, ">") == 0)
		{
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
			if (!list->next || !(*list->next->content))
			{
				printf(" Rien apres le <!\n");
				return (0);
			}
			data->cmd_args = create_cmd_args(data, list->index);
			handle_back(data, list->index);
			return (0);	
		}
		else if (ft_strlcmp(list->content, ">>") == 0)
		{
			if (!list->next || !(*list->next->content))
			{
				printf(" Rien apres le >>!\n");
				return (0);
			}
			data->cmd_args = create_cmd_args(data, list->index);
			handle_append(data, list->index);
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
	int		i;
	char	*path_test;
	
	i = 0;
	if (ft_empty(data) == 1)
		return (1);
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
	int	i;

	i = 0;	
	if (!data.input)
		return (1);
	while(data.input[i] == ' ')
		i++;
	if (data.input[i] == '\0')
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

void	ft_clean_input(t_data *data)
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
			ft_clean_input(data);
		i++;
	}
}

void	exec_command(t_data *data)
{
	pid_t	pid;
	int		status;
	
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		execve(data->command_path, data->args, data->envp);
		perror("execve");
		exit(127);
	}	
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status)) // cas ou on a fait ctrl +c pendant la commande (par ex un sleep), la ca a pas vraiment
	// dinteret cest juste pour faire comme bash
	{
		int signal_number = WTERMSIG(status);
		printf("Process terminated by signal: %d\n", signal_number);
		data->exit_status = 128 + signal_number;
	}
	else if (WIFEXITED(status)) //cas ou tout se passe bien
	{
		data->exit_status = WEXITSTATUS(status);
		//check_exit_status(data);
	}
	else //securite au cas ou yai un truc innatendu, afin que exit status ai pas une valeur random
	{
		data->exit_status = 128;
		//check_exit_status(data);
	}
	return;
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

void	parsing(char *input, char **envp, t_data *data)
{
	pid_t	pid;
	int		status;
	
	if (count_global_quotes(data->input) % 2 != 0) // permet de gerer le cas de quotes non fermees
	{
		data->exit_status = 127;
		printf("minishell: unclosed quote detected\n");
		return ;
	}
	// delete_quotes_inside(data); //tout ce que jai mis en comm je laisse au cas ou
	if (check_empty(*data) == 1) // cas ou on appuie juste sur entree
	{
		return ;
	}
	if (check_builtin(data) != 0) //la on verifie si ya on est dans le cas dun built in
		return;
	// delete_quotes_hard(data);
	if (input[0] == '/' || (input[0] == '.' && input[1] == '/')) //ce cas la est pas demande dans le sujet, donc on pourra enlever
	//je pense mais pour linstant je le garde au cas ou mais tu peux passer
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
		}
		return;
	}
	data->path = ft_split(get_path_env(envp), ':');//a partir de la cest comme pipex finalement
	data->path = add_slash_all(data->path);
	if (test_commands(data) == 0)// cas ou on trouve la commande avec access
	{
		exec_command(data); //execute la commande.
		return;
	}
	else //cas ou on trouve pas
	{
		data->exit_status = 127;
		printf("minishell: %s: command not found\n", data->list->content);
	}
}
