/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 19:31:51 by maissat           #+#    #+#             */
/*   Updated: 2025/03/31 20:03:40 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int	check_export_compatibility(char *str)
// {
// 	int	i;
// 	int	quote;
	
// 	i = 0;
// 	quote = 0;
// 	while (is_alpha(str[i]))
// 		i++;
// 	if (str[i] == '=')
// 		i++;
// 	else
// 		return (1);
// 	if (str[i] == '"')
// 		quote = 1;
// 	i++;
// 	while (is_alpha(str[i]) || str[i] == '/')
// 		i++;
// 	if (str[i] == '\0')
// 		return (0);
// 	else if ((quote == 1 && str[i] == '"'))
// 		return (0);
// 	else
// 		return (1);
// }

int	check_export_compatibility(char *str)
{
	int i;

	if (!str || (!is_alpha(str[0]) && str[0] != '_')) // Doit commencer par une lettre ou un _
		return (1);
	i = 1;
	while (str[i] && (is_alphanum(str[i]) || str[i] == '_')) // Lettres, chiffres ou _
		i++;
	if (str[i] == '=' || str[i] == '\0') // Fin correcte (fin de variable ou début de valeur)
		return (0);
	return (1);
}

void	show_tab_export(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		printf("export %s\n", tab[i]);
		i++;
	}
}

int	ft_export(t_cmd *cmd, t_data *data)
{
	int		i;
	char	**args;
	int		exit_status;

	exit_status = 0;
	args = cmd->args;
	if (count_args(args) == 1)
		return (show_tab_export(data->envp), 0);
	i = 1;
	while (args[i])
	{
		if (check_export_compatibility(args[i]) == 0)
		{
			if (check_change(data, cmd->args[i]) == 0)
				data->envp = add_export(data, cmd->args[i]);
		}
		else
		{
			printf("Bad format!\n");
			exit_status = 1;
		}
		i++;
	}
	return (exit_status);
}