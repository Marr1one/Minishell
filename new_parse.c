/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:30:52 by maissat           #+#    #+#             */
/*   Updated: 2025/03/21 21:20:47 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirect(char c)
{
	if (c == '>' || c == '<')
		return (1);
	return (0);
}

int	is_pipe(char c)
{
	if (c == '|')
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

/*
 * expand_string : traite une chaîne et effectue l'expansion des variables
 * selon les règles définies.
 * - Les '$' sont traités en comptant le nombre de '$' consécutifs.
 * - Si le nombre est pair, on insère (count/2) dollars littéraux.
 * - Si le nombre est impair, on insère ((count-1)/2) dollars littéraux,
 *   puis on lit le nom de la variable (lettres, chiffres, ou '_')
 *   et on remplace par sa valeur obtenue avec getenv.
 *
 * data n'est pas utilisé ici directement, mais vous pouvez l'ajouter
 * si vous avez une version personnalisée de getenv ou d'autres besoins.
 */
char *expand_string(const char *input)
{
    char *result;
    int result_size;
    int result_len;
    int i, count, start;
    char *temp;
    char *var_value;
    
    result_size = 128;  /* Taille initiale du buffer */
    result = malloc(result_size);
    if (!result)
        return NULL;
    result[0] = '\0';
    result_len = 0;
    i = 0;
    while (input[i])
    {
        if (input[i] != '$')
        {
            /* Ajout du caractère courant */
            if (result_len + 2 > result_size)
            {
                result_size *= 2;
                temp = realloc(result, result_size);
                if (!temp) { free(result); return NULL; }
                result = temp;
            }
            result[result_len] = input[i];
            result_len++;
            result[result_len] = '\0';
            i++;
        }
        else
        {
            /* On est sur un '$' : compter le nombre consécutif */
            count = 0;
            while (input[i] == '$')
            {
                count++;
                i++;
            }
            if (count % 2 == 0)
            {
                /* Nombre pair : insérer count/2 dollars littéraux */
                int j = 0;
                while (j < count / 2)
                {
                    if (result_len + 2 > result_size)
                    {
                        result_size *= 2;
                        temp = realloc(result, result_size);
                        if (!temp) 
						{
							free(result);
							return (NULL);
						}
                        result = temp;
                    }
                    result[result_len] = '$';
                    result_len++;
                    j++;
                }
                result[result_len] = '\0';
            }
            else
            {
                /* Nombre impair : insérer (count-1)/2 dollars littéraux */
                int j = 0;
                while (j < (count - 1) / 2)
                {
                    if (result_len + 2 > result_size)
                    {
                        result_size *= 2;
                        temp = realloc(result, result_size);
                        if (!temp) { free(result); return NULL; }
                        result = temp;
                    }
                    result[result_len] = '$';
                    result_len++;
                    j++;
                }
                /* Maintenant, on effectue l'expansion.
                 * On lit le nom de la variable (lettres, chiffres ou '_')
                 */
                start = i;
                while (input[i] && (isalnum((unsigned char)input[i]) || input[i] == '_'))
                    i++;
                {
                    int var_len = i - start;
                    char *var_name = malloc(var_len + 1);
                    if (!var_name) { free(result); return NULL; }
                    strncpy(var_name, input + start, var_len);
                    var_name[var_len] = '\0';
                    /* Utilisation de getenv pour obtenir la valeur */
                    var_value = getenv(var_name);
                    free(var_name);
                    if (!var_value)
                        var_value = "";
                    /* Ajouter var_value à result */
                    int value_len = strlen(var_value);
                    while (result_len + value_len + 1 > result_size)
                    {
                        result_size *= 2;
                        temp = realloc(result, result_size);
                        if (!temp) 
						{ 
							free(result);
							return (NULL); 
						}
                        result = temp;
                    }
                    strcat(result, var_value);
                    result_len += value_len;
                }
            }
        }
    }
    return result;
}


void	expand_all(t_cmd *cmd, t_data *data)
{
	t_cmd	*current_cmd;
	int		i;
	char	*expanded;
	t_file	*current_file;

	current_cmd = cmd;
	(void)data;
	while (current_cmd)
	{
		i = 0;
		while (current_cmd->args && current_cmd->args[i])
		{
			expanded = expand_string(current_cmd->args[i]);
			free(current_cmd->args[i]);
			current_cmd->args[i] = expanded;
			i++;
		}
		current_file = current_cmd->files;
		while (current_file)
		{
			expanded = expand_string(current_file->path);
			free(current_file->path);
			current_file->path = expanded;
			current_file = current_file->next;
		}
		current_cmd = current_cmd->next;
	}
}
