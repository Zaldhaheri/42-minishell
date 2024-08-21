/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bcomm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 17:23:47 by zaldhahe          #+#    #+#             */
/*   Updated: 2024/08/21 23:17:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec/executor.h"

void	b_unset_del(t_env *prev, t_env *curr)
{
	if (prev)
		prev->next = curr->next;
	free(curr->key);
	free(curr->value);
	free(curr);
}

void	b_unset(t_data *data, char **cmd)
{
	t_env	*curr;
	t_env	*prev;
	t_env	*temp;
	int		i;

	i = 1;
	while (cmd[i])
	{
		curr = data->myenv;
		prev = NULL;
		while (curr)
		{
			if (curr->key && !ft_strcmp(curr->key, cmd[i])
				&& curr->key[0] != '?')
			{
				temp = curr;
				curr = curr->next;
				b_unset_del(prev, temp);
				break ;
			}
			prev = curr;
			curr = curr->next;
		}
		i++;
	}
}

void	b_cd_oldpwd(t_data *data, char **temp, char **curr_pwd)
{
	*temp = get_pwd();
	*curr_pwd = ft_strjoin("OLDPWD=", *temp);
	add_to_myenv(data, ft_strdup(*curr_pwd), 0, 1);
	free(*temp);
	free(*curr_pwd);
}

void	b_cd(t_data *data, char **cmd)
{
	char	*home;
	char	*curr_pwd;
	char	*temp;

	b_cd_oldpwd(data, &temp, &curr_pwd);
	home = get_env_value(data, "HOME");
	if (!cmd[1] || !ft_strncmp(cmd[1], "~", 1))
	{
		if (!home)
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		else
			chdir(home);
	}
	else if (chdir(cmd[1]) == -1)
	{
		perror("cd");
		data->status = 1;
	}
	temp = get_pwd();
	curr_pwd = ft_strjoin("PWD=", temp);
	add_to_myenv(data, ft_strdup(curr_pwd), 0, 1);
	free(curr_pwd);
	free(temp);
}

void	b_declare(t_data *data, char **cmd)
{
	int	i;
	int	valid;

	valid = 1;
	i = 0;
	while (cmd[i] && valid)
	{
		if (!((ft_strrchr(cmd[i], '=') && ft_strlen(cmd[i]) > 1
					&& cmd[i][0] != '=' && is_valid_key(cmd[i]))))
			valid = 0;
		i++;
	}
	if (valid)
	{
		i--;
		add_to_myenv(data, ft_strdup(cmd[i]), 1, 0);
	}
}
