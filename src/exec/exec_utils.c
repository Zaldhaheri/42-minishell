/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:24:22 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/23 18:11:31 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <errno.h>

void	exit_free(t_command *cmd, t_data *data, int exit_status)
{
	if (cmd)
		free_commands(&cmd);
	if (data)
	{
		if (data->myenvstr)
			free_args(data->myenvstr);
		ft_envclear(&data->myenv);
	}
	ft_lstclear(data);
	close_std();
	exit(exit_status);
}

void	exec_child(t_command *cmd, t_data *data, t_child_params *params)
{
	int	exit_status;

	exit_status = 1;
	if (cmd && cmd->command[0] && !cmd->is_bcommand)
	{
		execve(cmd->command[0], cmd->command, data->myenvstr);
		if (errno == ENOENT)
			exit_status = exec_printerr(cmd->command[0]);
		else
		{
			perror(cmd->command[0]);
			exit_status = 126;
		}
	}
	else if (cmd->is_bcommand)
	{
		bcomm_exec(cmd, data, params);
		exit_status = 0;
	}
	if (cmd->fd_out > -1)
		close(cmd->fd_out);
	if (cmd->fd_in > -1)
		close(cmd->fd_in);
	close(params->saved_stdout);
	exit_free(cmd, data, exit_status);
}

void	start_child(t_command *cmd, t_data *data, t_child_params *params)
{
	if (params->is_first && cmd->fd_in > -1)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
		cmd->fd_in = NO_FD;
	}
	else if (!params->is_first)
	{
		if (cmd->fd_in > -1)
			dup2(cmd->fd_in, STDIN_FILENO);
		else
			dup2(params->fd_in, STDIN_FILENO);
	}
	if (cmd->next && cmd->fd_out < 0)
		dup2(params->fd[1], STDOUT_FILENO);
	else if (cmd->fd_out > -1)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
	}
	if (params->fd[0] > -1)
		close(params->fd[0]);
	if (params->fd[1] > -1)
		close(params->fd[1]);
	exec_child(cmd, data, params);
}

static void	bcomm_parent(t_command *cmd, t_data *data, t_child_params	*params)
{
	int	saved_stdout;

	if (cmd->fd_out > -1)
		saved_stdout = dup(STDOUT_FILENO);
	if (cmd->fd_out > -1)
		dup2(cmd->fd_out, STDOUT_FILENO);
	bcomm_exec(cmd, data, params);
	if (cmd->fd_out > -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	if (cmd->fd_out == -1)
		data->status = 1;
	else if (!data->sflag)
		data->status = 0;
}

void	parent_pid(t_command *cmd, t_child_params *params, t_data *data)
{
	if (cmd->is_bcommand && !cmd->next && params->is_first)
	{
		bcomm_parent(cmd, data, params);
		return ;
	}
	if (cmd->next)
	{
		close(params->fd[1]);
		dup2(params->fd[0], STDIN_FILENO);
		close(params->fd[0]);
	}
	else if (!params->is_first)
	{
		dup2(params->saved_stdout, STDIN_FILENO);
		close(params->saved_stdout);
	}
	if (cmd->fd_out > -1)
		close(cmd->fd_out);
	if (cmd->fd_in > -1)
		close(cmd->fd_in);
	params->is_first = 0;
}
