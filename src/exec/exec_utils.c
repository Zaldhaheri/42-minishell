/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:24:22 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/20 23:21:19 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <errno.h>

void	exit_free(t_command *cmd, t_data *data, int exit_status)
{
	free_commands(&cmd);
	free_args(data->myenvstr);
	ft_envclear(&data->myenv);
	ft_lstclear(data);
	exit(exit_status);
}

void	exec_child(t_command *cmd, t_data *data, t_child_params *params)
{
	int	exit_status;

	exit_status = 1;
	if (params->fd[0] > -1)
		close(params->fd[0]);
	if (cmd && cmd->command[0] && !cmd->is_bcommand)
		execve(cmd->command[0], cmd->command, data->myenvstr);
	if (errno == ENOENT)
	{
		ft_putstr_fd(cmd->command[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	if (cmd->is_bcommand)
	{
		bcomm_exec(cmd, data);
		exit_status = 0;
	}
	if (params->fd[1] > -1)
		close(params->fd[1]);
	if (cmd->cmd_fd > 0)
	{
		close(cmd->cmd_fd);
	}
	exit_free(cmd, data, exit_status);
}

void	start_child(t_command *cmd, t_data *data, t_child_params *params)
{
	if (!cmd->next && params->is_first)
	{
		if (cmd->fd_type == FD_OUT || cmd->fd_type == APPEND)
			dup2(cmd->cmd_fd, STDOUT_FILENO);
		if (cmd->fd_type == FD_IN)
			dup2(cmd->cmd_fd, STDIN_FILENO);
		exec_child(cmd, data, params);
	}
	if (cmd->fd_type == FD_OUT || cmd->fd_type == APPEND)
		dup2(cmd->cmd_fd, STDOUT_FILENO);
	else if (cmd->next)
		dup2(params->fd[1], STDOUT_FILENO);
	if (!params->is_first)
	{
		if (cmd->fd_type == FD_IN)
			dup2(cmd->cmd_fd, STDIN_FILENO);
		else
			dup2(params->fd_in, STDIN_FILENO);
	}
	if (cmd->next)
		close(params->fd[0]);
	close(params->fd[1]);
	exec_child(cmd, data, params);
}

void	parent_pid(t_command *cmd, t_child_params	*params, t_data *data)
{
	// int	saved_stdout;

	if (cmd->is_bcommand && !cmd->next && params->is_first)
	{
		//saved_stdout = dup(STDOUT_FILENO);
		if (cmd->fd_type == FD_OUT || cmd->fd_type == APPEND)
			dup2(cmd->cmd_fd, STDOUT_FILENO);
		bcomm_exec(cmd, data);
		//dup2(saved_stdout, STDOUT_FILENO);
		// close(saved_stdout);
		if (cmd->cmd_fd == -1)
			data->status = 1;
		else
			data->status = 0;
		return ;
	}
	if (cmd->next)
	{
		close(params->fd[1]);
		if (cmd->fd_type == FD_IN)
			params->fd_in = cmd->cmd_fd;
		else
			params->fd_in = params->fd[0];
	}
	if (cmd->cmd_fd == -1)
		data->status = 1;
	if (cmd->cmd_fd > 0)
		close(cmd->cmd_fd);
	params->is_first = 0;
}

void	exec_cmd(t_command *cmd, t_data *data)
{
	pid_t			pid;
	t_child_params	params;

	params.fd_in = STDIN_FILENO;
	params.fd_out = STDOUT_FILENO;
	params.is_first = 1;
	params.fd[0] = -10;
	params.fd[1] = -10;
	while (cmd)
	{
		if (cmd->next)
			create_pipe(&params);
		if (!cmd->next && params.is_first && cmd->is_bcommand)
			parent_pid(cmd, &params, data);
		else
		{
			pid = fork();
			if (pid == -1)
				return ((perror("fork"), exit(EXIT_FAILURE)));
			if (pid == 0)
				start_child(cmd, data, &params);
			else
				parent_pid(cmd, &params, data);
		}
		cmd = cmd->next;
	}
	if (params.fd_in != STDIN_FILENO)
		close(params.fd_in);
	if (!params.is_first)
		waitpid(pid, &(data->status), 0);
}
