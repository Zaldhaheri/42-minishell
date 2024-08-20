/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 20:29:54 by zaldhahe          #+#    #+#             */
/*   Updated: 2024/08/20 20:30:26 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (s)
	{
		while (*s)
		{
			write(fd, s, 1);
			s++;
		}
	}
}

int	ft_numlen(long n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char		*itoa;
	int			len;
	long long	nl;

	nl = n;
	len = ft_numlen(nl);
	itoa = malloc(sizeof(char) * (len + 1));
	if (!(itoa))
		return (NULL);
	itoa[len--] = '\0';
	if (nl == 0)
		itoa[0] = '0';
	else if (nl < 0)
	{
		itoa[0] = '-';
		nl = -nl;
	}
	while (nl > 0)
	{
		itoa[len--] = (nl % 10) + 48;
		nl /= 10;
	}
	return (itoa);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	s;
	int	r;

	i = 0;
	r = 0;
	s = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			s *= -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		r *= 10;
		r += str[i++] - '0';
	}
	return (r * s);
}

void	*ft_memset(void *str, int x, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *) str;
	while (i < n)
		ptr[i++] = x;
	str = ptr;
	return (str);
}
