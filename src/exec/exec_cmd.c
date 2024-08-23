/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 09:43:08 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/23 18:10:24 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	params_init(t_child_params	*params)
{
	params->fd_in = STDIN_FILENO;
	params->fd_out = STDOUT_FILENO;
	params->is_first = 1;
	params->fd[0] = -10;
	params->fd[1] = -10;
	params->saved_stdout = dup(STDIN_FILENO);
}

static void	child_proc(t_data *data, t_child_params	*params)
{
	waitpid(params->pid, &data->status, 0);
	if (data->status == 11)
	{
		ft_putstr_fd("Segmentation fault: 11\n", 2);
		data->status = 139;
	}
	else
		data->status = WEXITSTATUS(data->status);
	if (data->sflag == 2)
		data->status = 1;
	data->sflag = 0;
	close(params->saved_stdout);
}

void	exec_cmd(t_command *cmd, t_data *data)
{
	t_child_params	params;

	params_init(&params);
	params.pid = 0;
	while (cmd)
	{
		if (cmd->next)
			create_pipe(&params);
		if (!cmd->next && params.is_first && cmd->is_bcommand)
			parent_pid(cmd, &params, data);
		else
		{
			params.pid = fork();
			if (params.pid == -1)
				return ((perror("fork"), exit(EXIT_FAILURE)));
			if (params.pid == 0)
				start_child(cmd, data, &params);
			else
				parent_pid(cmd, &params, data);
		}
		cmd = cmd->next;
	}
	if (params.fd_in != STDIN_FILENO)
		close(params.fd_in);
	child_proc(data, &params);
}

int	exec_printerr(char *cmd)
{
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	return (127);
}
