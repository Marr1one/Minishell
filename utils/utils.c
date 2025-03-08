/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maissat <maissat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:26:21 by maissat           #+#    #+#             */
/*   Updated: 2025/03/08 17:40:13 by maissat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**copy_env(char **envp)
{
	int	i;
	char	**new_env;

	i = 0;
	while(envp[i])
		i++;
	new_env = ft_malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

//char	*remove_utils(char *str)
//{
//	int		i;
//	int		j;
//	char	*new_str;
	
//	new_str = malloc(sizeof(char) * ft_strlen(str));
//	if (!new_str)
//		return (NULL);
//	i = 0;
//	j = 0;
//	while (str[i])
//	{
//		if (str[i] == '"' || str[i] == '\'')
//		{
//			i++;
//		}
//		else
//		{
//			new_str[j] = str[i];
//			i++;
//			j++;
//		}
//	}
//	new_str[j] = '\0';
//	//printf("new_str = {%s}\n", new_str);
//	return (new_str);	
	
//}

char	**cut_last(char **tab, int	i)
{
	char	**new_tab;
	int		j;
	
	j = 0;
	new_tab = ft_malloc(sizeof(char *) * (i + 1));
	if (!new_tab)
		return (NULL);
	while (j < i)
	{
		new_tab[j] = tab[j];
		j++;
	}
	new_tab[j] = NULL;
	return (new_tab);
}

//void	remove_empty_quotes(t_data *data)
//{
//	int		i;
//    int 	j;
//	char	**args;

//	args = data->args;
//    while (args[i])
//    {

//        if (ft_strcmp(args[i], "\"\"") == 0)
//        {
//            // Si c'est une paire de guillemets vides, supprime cet argument
//            free(args[i]);
//            // Décale les éléments suivants
//            while (args[i + 1])
//            {
//                args[i] = args[i + 1];
//                i++;
//            }
//            args[i] = NULL;
//        }
//        else
//        {
//            i++;
//        }
//    }
//}



int	ft_strlen(const char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_strncmp(char *s1, char *s2, int n)
{
	int	i;

	i = 0;
	if (!s1 || !s2 || n < 0)
		return (-1);
	while (s1[i] && s2[i] && i < n)
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	return (0);
}

int	check_len(char *s1, char *s2)
{
	int len_s1;
	int	len_s2;
	
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	if (len_s1 != len_s2)
		return (1);
	return (0);
}

int	ft_strlcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	if (!(*s1) || !(*s2))
		return (1);
	if (check_len(s1, s2) != 0)
		return (1);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	if (!(*s1) || !(*s2))
		return (1);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	return (0);
}

char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

void	*ft_memset(void *b, int c, size_t len)
{
	char	*start;

	if (b == NULL)
		return (NULL);
	start = b;
	while (len)
	{
		*start = c;
		start++;
		len--;
	}
	return (b);
}

char	*ft_strdup(char	*str)
{
	int		i;
	char	*dup;

	i = 0;
	dup = ft_malloc(sizeof(char) * ft_strlen(str) + 1);
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_join(char *str, char *add)
{
	int		i;
	int		j;
	char	*join;

	i = 0;
	j = 0;
	if (!str || !add)
		return (NULL);
	join = ft_malloc(sizeof(char) * (ft_strlen(str) + ft_strlen(add) + 1));
	if (!join)
		return (NULL);
	while (str[i])
	{
		join[i] = str[i];
		i++;
	}
	j = 0;
	while (add[j])
	{
		join[i] = add[j];
		i++;
		j++;
	}
	join[i] = 0;
	return (join);
}

char	*ft_itoa(int	n)
{
	char	*res;
	int		tmp;
	int		len;

	len = 0;
	tmp = n;
	if (n == 0)
    {
        res = ft_malloc(sizeof(char) * 2);
        if (!res)
            return (NULL);
        res[0] = '0';
        res[1] = '\0';
        return (res);
    }
	while (tmp)
	{
		tmp = tmp / 10;
		len++;
	}
	res = ft_malloc(sizeof(char) * 2);
	if (!res)
		return (NULL);
	//res[len] = '\0'
	while(len --)
	{
		res[len] = '0' + (n % 10);
		n = n / 10;
	}
	return (res);
}

int	ft_atoi(char	*str)
{
	int	n;
	int	i;
	
	n = 0;
	i = 0;
	while (str[i])
	{
		n = n * 10 + (str[i] - '0');
		i++;
	}
	return (n);
}
char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	j;

	if (!*needle)
		return ((char *)haystack);
	i = 0;
	while (haystack[i])
	{
		j = 0;
		while (haystack[i + j] && needle[j]
			&& haystack[i + j] == needle[j])
			j++;
		if (!needle[j])
			return ((char *)(haystack + i));
		i++;
	}
	return (0);
}
