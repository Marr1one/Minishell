/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 22:00:55 by braugust          #+#    #+#             */
/*   Updated: 2025/03/21 17:35:52 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *ft_strchr(const char *s, int c)
{
        int     i;

        i = 0;
        while (s[i])
        {
                if (s[i] == (char)c)
                        return ((char *)(&s[i]));
                i++;
        }
        if (s[i] == (char)c)
                return ((char *)(&s[i]));
        return (0);
}

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}

char	*ft_strtrim(char const *s)
{
	size_t	start;
	size_t	end;
	size_t	i;
	char	*trimmed;

	if (!s)
		return (NULL);
	start = 0;
	while (s[start] && is_whitespace(s[start]))
		start++;
	end = ft_strlen(s);
	while (end > start && is_whitespace(s[end - 1]))
		end--;
	trimmed = ft_malloc((end - start + 1) * sizeof(char));
	if (!trimmed)
		return (NULL);
	i = 0;
	while (start < end)
		trimmed[i++] = s[start++];
	trimmed[i] = '\0';
	return (trimmed);
}

char *get_cmd_path(const char *cmd, char **envp)
{
    (void)envp;
    size_t len;
    char *path;

    len = ft_strlen("/bin/") + ft_strlen(cmd) + 1;
    path = ft_malloc(len);
    if (!path)
        return (NULL);
    strcpy(path, "/bin/");
    strcat(path, cmd);
    if (access(path, X_OK) == 0)
        return (path);
    return (NULL);
}

// void execute_pipex(t_data *data)
// {
//     char                    **cmds;
//     t_pipex_pipe            *pipes;
//     char                    **args;
//     char                    *cmd_path;
//     pid_t                   pid;
//     pid_t                   last_pid;
//     int                     nb_cmd;
//     int                     i;
//     int                     status;

//     cmds = ft_split(data->input, '|');
//     if (!cmds)
//         return ;
//     nb_cmd = count_tab(cmds);
//     i = 0;
//     while (i < nb_cmd)
//     {
//         cmds[i] = ft_strtrim(cmds[i]);
//         i++;
//     }  
//     pipes = init_pipes(nb_cmd);
//     last_pid = 0;
//     i = 0;
//     while (i < nb_cmd)
//     {
//         pid = fork();
//         if (pid < 0)
//         {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         }
//         if (pid == 0)
//         {
//             child_redirection(i, nb_cmd, pipes);
//             if (nb_cmd > 1)
//                 close_all_pipes(nb_cmd, pipes);
//             args = ft_split(cmds[i], ' ');
//             if (!args || !args[0])
//             {
//                 write(STDERR_FILENO, "Error: empty command\n", 21);
//                 exit(127);
//             }
//             /* Vérifier si la commande est un builtin */
//             if (is_builtin(args[0]))
//             {
//                 execute_builtin(data, args);
//                 exit(data->exit_status);
//             }
//             cmd_path = get_cmd_path(args[0], data->envp);
//             if (!cmd_path)
//             {
//                 fprintf(stderr, "Command not found: %s\n", args[0]);
//                 exit(127);
//             }
//             free(args[0]);
//             args[0] = cmd_path;
//             execve(args[0], args, data->envp);
//             perror("execve");
//             exit(127);
//         }
//         i++;
//         last_pid = pid;
//     }
//     if (nb_cmd > 1 && pipes)
//     {
//         close_all_pipes(nb_cmd, pipes);
//         free(pipes);
//     }
//     if (waitpid(last_pid, &status, 0) < 0)
//         status = 1;
//     while (wait(NULL) > 0)
//         ;
// }

int count_tab(char **tab)
{
    int count = 0;
    while (tab && tab[count])
        count++;
    return count;
}