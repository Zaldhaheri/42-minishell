/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:24:22 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/22 17:44:03 by nalkhate         ###   ########.fr       */
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
		{
			ft_putstr_fd(cmd->command[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
		}
		exit_status = 127;
	}
	else if (cmd->is_bcommand)
	{
		bcomm_exec(cmd, data, params);
		exit_status = 0;
	}
	if (cmd->cmd_fd > 0)
		close(cmd->cmd_fd);
	close(params->saved_stdout);
	exit_free(cmd, data, exit_status);
}

void	start_child(t_command *cmd, t_data *data, t_child_params *params)
{
	if (params->is_first && cmd->fd_type == FD_IN)
	{
		dup2(cmd->cmd_fd, STDIN_FILENO);
		close(cmd->cmd_fd);
		cmd->cmd_fd = -1;
	}
	else if (!params->is_first)
	{
		dup2(params->fd_in, STDIN_FILENO);
	}
	if (cmd->next)
		dup2(params->fd[1], STDOUT_FILENO);
	else if (cmd->fd_type == FD_OUT || cmd->fd_type == APPEND)
	{
		dup2(cmd->cmd_fd, STDOUT_FILENO);
		close(cmd->cmd_fd);
		cmd->cmd_fd = -1;
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

	if (cmd->fd_type == FD_OUT || cmd->fd_type == APPEND)
		saved_stdout = dup(STDOUT_FILENO);
	if (cmd->fd_type == FD_OUT || cmd->fd_type == APPEND)
		dup2(cmd->cmd_fd, STDOUT_FILENO);
	bcomm_exec(cmd, data, params);
	if (cmd->fd_type == FD_OUT || cmd->fd_type == APPEND)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	if (cmd->cmd_fd == -1)
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
	if (cmd->cmd_fd > 0)
		close(cmd->cmd_fd);
	params->is_first = 0;
}
