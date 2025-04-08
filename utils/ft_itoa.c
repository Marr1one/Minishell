/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: braugust <braugust@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:10:46 by braugust          #+#    #+#             */
/*   Updated: 2025/04/08 19:10:10 by braugust         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../minishell.h"

static int	ft_numlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static void	ft_fillstr(char *res, int n, int len)
{
	int	sign;

	sign = 1;
	if (n < 0)
	{
		sign = -1;
		n = -n;
	}
	res[len] = '\0';
	while (len--)
	{
		res[len] = (n % 10) + '0';
		n = n / 10;
	}
	if (sign == -1)
		res[0] = '-';
}

char	*ft_itoa(int n)
{
	char	*res;
	int		len;

	len = ft_numlen(n);
	res = ft_malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	ft_fillstr(res, n, len);
	return (res);
}

char	**create_new_args(char **args, int idx)
{
	int		i;
	int		j;
	char	**res;

	i = 0;
	j = 0;
	res = ft_malloc(sizeof(char) * count_args(args));
	while (args[i])
	{
		if (i != idx)
			res[j++] = args[i];
		i++;
	}
	res[j] = NULL;
	return (res);
}
