/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingenv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 20:23:28 by zaldhahe          #+#    #+#             */
/*   Updated: 2024/08/20 20:23:28 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_valid_key(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (ft_isalnum(str[i]) || str[i] == '_')
		{
			i++;
		}
		else
		{
			return (0);
		}
	}
	return (1);
}

int	check_env_dupes(t_data *data, char *str, int sethide)
{
	int		flag;
	char	**split;
	t_env	*curr;

	curr = data->myenv;
	flag = 1;
	split = ft_split(str, '=');
	data->temp = split[0];
	while (curr)
	{
		if (curr->key && !ft_strcmp(data->temp, curr->key))
		{
			free(curr->value);
			curr->value = ft_strdup(split[1]);
			if (sethide)
				curr->hide = 0;
			flag = 0;
			break ;
		}
		curr = curr->next;
	}
	if (split)
		free_split_from(split, 0);
	return (flag);
}

void	add_token_to_env(t_data *data)
{
	if (check_env_dupes(data, data->checker, 0))
		ft_envadd_back(&data->myenv, data->checker, 1);
	if (data->checker)
		free(data->checker);
	data->checker = ft_strdup("");
}

char	*get_env_value(t_data *data, char *key)
{
	t_env	*curr;

	curr = data->myenv;
	if (!key)
		return (NULL);
	while (curr)
	{
		if (curr->key && !ft_strcmp(key, curr->key))
			return (curr->value);
		curr = curr->next;
	}
	return (NULL);
}
