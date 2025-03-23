/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:30:52 by maissat           #+#    #+#             */
/*   Updated: 2025/03/23 19:15:26 by braugust         ###   ########.fr       */
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

char *expand_string(const char *arg)
{
    char    *result;
    char    *var_value;
    char    *var_name;
    int     i;
    int     j;
    int     k;
    int     start;
    int     var_len;
    int     final_len;
    

    final_len = 0;
    i = 0;
    // Calcul de la longeur de la chaine final result
    while (arg[i])
    {
        if (arg[i] != '$')
        {
            final_len++;
            i++;
        }
        else
        {
            i++;
            start = i;
            while (arg[i] && (isalnum((unsigned char)arg[i]) || arg[i] == '_'))
                i++;
            var_len = i - start;
            if (var_len > 0)
            {
                var_name = ft_malloc(var_len + 1);
                if (!var_name)
                    return(NULL);
                k = 0;
                while (k < var_len)
                {
                    var_name[k] = arg[start + k];
                    k++;
                }
                var_name[var_len] = '\0';
            }
            else
                var_name = NULL;
            if (var_name)
                var_value = getenv(var_name);
            else
                var_value = NULL;
            if (!var_value)
                var_value = "";
            final_len += (int)ft_strlen(var_value);
            if (var_name)
                free(var_name);
        }
    }
    // on alloue resultat avec la taille final calcule 
    result = ft_malloc(final_len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (arg[i])
	{
		if (arg[i] != '$')
		{
			result[j] = arg[i];
			j++;
			i++;
		}
		else
		{
			i++;
			start = i;
			while (arg[i] && (isalnum((unsigned char)arg[i]) || arg[i] == '_'))
				i++;
			var_len = i - start;
			if (var_len > 0)
			{
				var_name = ft_malloc(var_len + 1);
				if (!var_name)
				{
					free(result);
					return (NULL);
				}
				k = 0;
				while (k < var_len)
				{
					var_name[k] = arg[start + k];
					k++;
				}
				var_name[var_len] = '\0';
			}
			else
				var_name = NULL;
			if (var_name)
				var_value = getenv(var_name);
			else
				var_value = NULL;
			if (!var_value)
				var_value = "";
			k = 0;
			while (var_value[k])
			{
				result[j] = var_value[k];
				j++;
				k++;
			}
			if (var_name)
				free(var_name);
		}
	}
	result[j] = '\0';
    return (result);
}


void	expand_all(t_cmd *cmd)
{
	t_cmd	*current_cmd;
	int		i;
	char	*expanded;
	t_file	*current_file;

	current_cmd = cmd;
	while (current_cmd)
	{
		i = -1;
		while (current_cmd->args && current_cmd->args[++i])
		{
			expanded = expand_string(current_cmd->args[i]);
			free(current_cmd->args[i]);
			current_cmd->args[i] = expanded;
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


// char    *expand_argument(const char *arg)
// {
//     char    *result;
//     char    *var_value;
//     int     i;
//     int     j;
//     int     start;

//     // Allouer une première version du résultat (max taille d'entrée)
//     result = ft_malloc(strlen(arg) * 2 + 1);
//     if (!result)
//         return (NULL);
//     i = 0;
//     j = 0;
//     while (arg[i])
//     {
//         if (arg[i] == '$')
//         {
//             i++;
//             start = i;
//             while (arg[i] && (isalnum((unsigned char)arg[i]) || arg[i] == '_'))
//                 i++;
//             if (i > start) // On a trouvé un nom de variable
//             {
//                 char var_name[i - start + 1];
//                 strncpy(var_name, arg + start, i - start);
//                 var_name[i - start] = '\0';
//                 var_value = getenv(var_name);
//                 if (!var_value)
//                     var_value = "";
//                 ft_strcpy(result + j, var_value);
//                 j += ft_strlen(var_value);
//             }
//             else // Juste un dollar isolé
//                 result[j++] = '$';
//         }
//         else
//             result[j++] = arg[i++];
//     }
//     result[j] = '\0';
//     return (result);
// }

// void    expand_all(t_cmd *cmd)
// {
//     t_cmd   *current_cmd;
//     int     i;
//     char    *expanded;

//     current_cmd = cmd;
//     while (current_cmd)
//     {
//         i = 0;
//         while (current_cmd->args && current_cmd->args[i])
//         {
//             expanded = expand_argument(current_cmd->args[i]);
//             free(current_cmd->args[i]);
//             current_cmd->args[i] = expanded;
//             i++;
//         }
//         current_cmd = current_cmd->next;
//     }
// }
