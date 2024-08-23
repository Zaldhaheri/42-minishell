/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 20:32:18 by zaldhahe          #+#    #+#             */
/*   Updated: 2024/08/23 16:43:59 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t n)
{
	size_t	i;
	size_t	slen;

	i = 0;
	slen = 0;
	while (src[slen])
		slen++;
	if (n == 0)
		return (slen);
	while (src[i] && i < n - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (slen);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char) s1[i] - (unsigned char) s2[i]);
}

int	ft_isalnum(int c)
{
	return ((c >= 97 && c <= 122) || (c >= 65 && c <= 90)
		|| (c >= 48 && c <= 57));
}

void	set_filename(t_token *tokens)
{
	t_token	*temp;

	temp = tokens;
	while (temp)
	{
		if (temp->next && (temp->type == FD_IN || temp->type == FD_OUT
				|| temp->type == APPEND))
		{
			temp->next->type = FILENAME;
		}
		else if (temp->next && temp->type == HEREDOC)
		{
			temp->next->type = LIMITER;
		}
		temp = temp->next;
	}
}
