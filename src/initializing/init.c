/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 19:49:24 by zaldhahe          #+#    #+#             */
/*   Updated: 2024/08/20 19:49:24 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_split_from(char **split, int from)
{
	while (split[from])
	{
		free(split[from]);
		from++;
	}
	free(split);
}

void	pre_init(t_data *data, char **envp)
{
	data->envp = envp;
	data->myenv = NULL;
	data->myenv = NULL;
	data->tokens = NULL;
	data->sflag = 0;
	data->i = 0;
	env_init(data);
	add_to_myenv(data, ft_strdup("?=0"), 1, 0);
	incr_shlvl(data);
	set_pwd(data);
}

void	data_init(t_data *data)
{
	data->i = 0;
	data->checker = ft_strdup("");
	data->tokens = NULL;
	data->typeflag = WORD;
	data->status = 0;
	data->myenvstr = env_to_array(data->myenv);
}

int	ft_envfullsize(t_env *myenv)
{
	t_env	*curr;
	int		i;

	i = 0;
	curr = myenv;
	while (curr)
	{
		if (curr->key && curr->value)
			i++;
		curr = curr->next;
	}
	return (i);
}

char	**env_to_array(t_env *myenv)
{
	int		size;
	char	*temp;
	char	**array;
	t_env	*curr;
	int		i;

	i = 0;
	size = ft_envfullsize(myenv);
	if (size == 0)
		return (NULL);
	array = malloc((size + 1) * sizeof(char *));
	curr = myenv;
	while (i < size)
	{
		if (curr->key && curr->value)
		{
			temp = ft_strjoin(curr->key, "=");
			array[i] = ft_strjoin(temp, curr->value);
			free(temp);
			i++;
		}
		curr = curr->next;
	}
	array[size] = NULL;
	return (array);
}
