/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envset.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 19:51:18 by zaldhahe          #+#    #+#             */
/*   Updated: 2024/08/20 19:51:18 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	set_exitstatus(t_data *data)
{
	char	*status_str;
	char	*exit_status;
	char	*temp;

	status_str = ft_itoa(data->status);
	temp = ft_strjoin("?", "=");
	exit_status = ft_strjoin(temp, status_str);
	free(status_str);
	free(temp);
	add_to_myenv(data, exit_status, 1, 0);
}

void	set_pwd(t_data *data)
{
	char	*str;
	char	*finalstr;
	char	*temp;

	str = getcwd(NULL, PATH_MAX);
	temp = ft_strjoin("PWD", "=");
	finalstr = ft_strjoin(temp, str);
	free(str);
	free(temp);
	add_to_myenv(data, finalstr, 0, 0);
}

void	incr_shlvl(t_data *data)
{
	char	*shlvl_str;
	char	*new_shlvl_str;
	int		shlvl;
	char	*value;

	value = get_env_value(data, "SHLVL");
	if (!value)
		shlvl = 1;
	else
		shlvl = ft_atoi(value) + 1;
	shlvl_str = ft_itoa(shlvl);
	new_shlvl_str = ft_strjoin("SHLVL=", shlvl_str);
	add_to_myenv(data, new_shlvl_str, 0, 0);
	if (shlvl_str)
		free(shlvl_str);
}

void	env_init(t_data *data)
{
	if (data->myenv)
		return ;
	data->myenv = NULL;
	while (data->envp[data->i])
	{
		ft_envadd_back(&data->myenv, data->envp[data->i], 0);
		data->i++;
	}
	data->i = 0;
}

void	add_to_myenv(t_data *data, char *str, int hide, int sethide)
{
	if (check_env_dupes(data, str, sethide))
		ft_envadd_back(&data->myenv, str, hide);
	if (str)
		free (str);
}
