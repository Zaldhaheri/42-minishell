/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 20:27:21 by zaldhahe          #+#    #+#             */
/*   Updated: 2024/08/20 20:27:31 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	is_sep_c(char c, char sep)
{
	if (c == sep)
		return (1);
	else
		return (0);
}

static int	count_str(char const *str, char sep)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (is_sep_c(str[i], sep) == 0
			&& (is_sep_c(str[i + 1], sep) == 1 || str[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static char	*add_word(char const *str, int len)
{
	char	*word;
	int		i;

	i = 0;
	word = (char *)malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (i < len)
	{
		word[i] = *str++;
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	split_words(char **splitted, int words, char const *s, char c)
{
	int		w;
	int		j;

	w = 0;
	j = 0;
	while (w < words)
	{
		j = 0;
		while (*s && is_sep_c(*s, c) == 1)
			s++;
		while (*s && is_sep_c(*s, c) == 0)
		{
			s++;
			j++;
		}
		splitted[w++] = add_word(s - j, j);
	}
}

char	**ft_split(char const *s, char c)
{
	char	**splitted;
	int		words;

	if (!s)
		return (NULL);
	words = count_str(s, c);
	splitted = (char **) malloc ((words + 1) * sizeof(char *));
	if (!splitted)
		return (NULL);
	split_words(splitted, words, s, c);
	splitted[words] = NULL;
	return (splitted);
}
