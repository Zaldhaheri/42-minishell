/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:20:57 by zaldhahe          #+#    #+#             */
/*   Updated: 2024/08/23 17:14:00 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec/executor.h"

void	b_env(t_data *data)
{
	t_env	*curr;

	curr = data->myenv;
	while (curr)
	{
		if (curr->key && curr->value && !curr->hide)
			printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
}

void	b_sort(char **arr, int size)
{
	int		swap;
	char	*temp;
	int		i;

	swap = 1;
	while (swap)
	{
		i = 0;
		swap = 0;
		while (i < size - 1)
		{
			if (ft_strcmp(arr[i], arr[i + 1]) > 0)
			{
				temp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = temp;
				swap = 1;
			}
			i++;
		}
	}
}

char	**exp_to_array(t_env *myenv)
{
	int		size;
	char	*temp;
	char	**array;
	t_env	*curr;
	int		i;

	i = 0;
	size = ft_envsize(myenv);
	if (size == 0)
		return (NULL);
	array = malloc((size + 1) * sizeof(char *));
	curr = myenv;
	while (i < size)
	{
		if (curr->key && curr->value && !curr->hide)
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

void	print_export(t_data *data)
{
	char	**tempenv;
	int		i;

	i = 0;
	tempenv = exp_to_array(data->myenv);
	b_sort(tempenv, ft_envsize(data->myenv));
	while (tempenv[i])
	{
		printf("%s\n", tempenv[i++]);
	}
	free_split_from(tempenv, 0);
}

void	b_export(t_data *data, char **cmd)
{
	int	i;

	i = 0;
	if (!cmd[1])
		print_export(data);
	else
	{
		while (cmd[i])
		{
			if (ft_strrchr(cmd[i], '=') && ft_strlen(cmd[i]) > 1
				&& cmd[i][0] != '=' && is_valid_key(cmd[i]))
				add_to_myenv(data, ft_strdup(cmd[i]), 0, 1);
			i++;
		}
	}
}
