/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bcomm_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 21:49:17 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/21 17:56:28 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec/executor.h"

void	bcomm_exec(t_command *cmd, t_data *data)
{
	if (!ft_strncmp("echo", cmd->command[0], 5))
		b_echo(data, cmd->command);
	else if (!ft_strncmp("env", cmd->command[0], 4))
		b_env(data);
	else if (!ft_strncmp("export", cmd->command[0], 7))
		b_export(data, cmd->command);
	else if (!ft_strncmp("unset", cmd->command[0], 6))
		b_unset(data, cmd->command);
	else if (!ft_strncmp("cd", cmd->command[0], 3))
		b_cd(data, cmd->command);
	else if (!ft_strncmp("pwd", cmd->command[0], 4))
		b_pwd();
	else if (!ft_strncmp("exit", cmd->command[0], 5))
		b_exit(data, cmd);
	else if (ft_strrchr(cmd->command[0], '=') && ft_strlen(cmd->command[0]) > 1
		&& cmd->command[0][0] != '=' && is_valid_key(cmd->command[0]))
		b_declare(data, cmd->command);
	else
	{
		printf("minishell: %s: command not found\n", cmd->command[0]);
		data->status = 256;
		data->sflag = 1;
	}
}
