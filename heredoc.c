/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 05:13:51 by braugust          #+#    #+#             */
/*   Updated: 2025/04/01 12:26:43 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_heredoc(t_cmd *current_cmd)
{
    int heredoc_pipe[2];
    char *heredoc_content;

    if (contains_heredoc(current_cmd)) {
        heredoc_content = execute_last_heredoc(current_cmd);
        if (heredoc_content != NULL) {
            if (pipe(heredoc_pipe) == -1) {
                perror("pipe");
                exit(1);
            }
            /* Écriture du contenu heredoc dans le pipe */
            write(heredoc_pipe[1], heredoc_content, ft_strlen(heredoc_content));
            close(heredoc_pipe[1]);
            /* Rediriger STDIN vers le pipe qui contient le heredoc */
            dup2(heredoc_pipe[0], STDIN_FILENO);
            close(heredoc_pipe[0]);
            //free(heredoc_content);
        }
    }
}

// Vérifie si une commande contient au moins un heredoc.
int contains_heredoc(t_cmd *cmd)
{
    t_file *current = cmd->files;
    while (current)
    {
        if (current->mode == HEREDOC)
            return (1);
        current = current->next;
    }
    return (0);
}

// Renvoie le dernier heredoc trouvé dans la liste des fichiers.
t_file *find_last_heredoc(t_file *files)
{
    t_file *current;
    t_file *last;

    current = files;
    last = NULL;
    while (current)
    {
        if (current->mode == HEREDOC)
            last = current;
        current = current->next;
    }
    return (last);
}
// Lit via readline jusqu'au délimiteur et retourne le contenu lu.
char *heredoc_loop(char *delimiter, char *prompt)
{
    char *content;
    char *line;
    char *tmp;

    content = ft_strdup("");
    while ((line = readline(prompt)) != NULL)
    {
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        tmp = content;
        content = ft_strjoin(content, line);
        //free(tmp);
        tmp = content;
        content = ft_strjoin(content, "\n");
        //free(tmp);
        free(line);
    }
    return (content);
}
// Ouvre le tty et lit le heredoc en redirigeant temporairement STDIN.
char *read_heredoc_from_tty(char *delimiter, char *prompt)
{
    char *content;
    int tty_fd;
    int std_backup;

    tty_fd = open("/dev/tty", O_RDONLY);
    if (tty_fd == -1)
    {
        perror("open /dev/tty");
        return (ft_strdup(""));
    }
    std_backup = dup(STDIN_FILENO);
    dup2(tty_fd, STDIN_FILENO);
    // printf("> (terminer avec '%s')\n", delimiter);
    content = heredoc_loop(delimiter, prompt);
    dup2(std_backup, STDIN_FILENO);
    close(std_backup);
    close(tty_fd);
    return (content);
}
// Cherche récursivement un heredoc existant dans la liste.
t_file *find_existing_heredoc(t_file *node)
{
    if (node == NULL)
        return (NULL);
    if (node->mode == HEREDOC)
        return (node);
    return (find_existing_heredoc(node->next));
}

// Renvoie récursivement le dernier noeud d'une liste de fichiers.
t_file *find_last_node(t_file *node)
{
    if (node == NULL)
        return (NULL);
    if (node->next == NULL)
        return (node);
    return (find_last_node(node->next));
}

// Crée un nouveau noeud heredoc à partir d'un token.
t_file *create_new_heredoc_node(t_token *token)
{
    t_file *new_node;

    new_node = ft_malloc(sizeof(t_file));
    if (new_node == NULL)
        return (NULL);
    new_node->mode = HEREDOC;
    new_node->path = ft_strdup(token->content);
    new_node->next = NULL;
    return (new_node);
}

// Ajoute ou remplace un heredoc dans la liste selon le token fourni.
t_file *add_or_replace_heredoc(t_file *files, t_token *heredoc_token)
{
    t_file *found;
    t_file *last;
    t_file *new_node;

    found = find_existing_heredoc(files);
    if (found != NULL)
    {
        //free(found->path);
        found->path = ft_strdup(heredoc_token->content);
        return (files);
    }
    new_node = create_new_heredoc_node(heredoc_token);
    if (new_node == NULL)
        return (files);
    if (files == NULL)
        return (new_node);
    last = find_last_node(files);
    if (last != NULL)
        last->next = new_node;
    return (files);
}

// Exécute le heredoc en récupérant le contenu du dernier heredoc de la commande.
char *execute_last_heredoc(t_cmd *cmd)
{
    t_file *last;
    char *content;

    last = find_last_heredoc(cmd->files);
    if (last == NULL)
        return (NULL);
    content = read_heredoc_from_tty(last->path, "> ");
    return (content);
}
