/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:24:22 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/18 15:32:12 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "executor.h"
#include <errno.h>


void exec_child(t_command *cmd, t_data *data, char **envp, t_child_params	*params)
{
	int exit_status;

	exit_status = 1;
	if (cmd && cmd->command[0] && !cmd->is_bcommand)
		execve(cmd->command[0], cmd->command, envp);

	if (errno == ENOENT)
	{
		ft_putstr_fd(cmd->command[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	else 
        perror(cmd->command[0]);
	if (cmd->is_bcommand)
	{
		ft_putstr_fd("-----BCOMM-----\n",2);
		bcomm_exec(cmd, data);
		exit_status = 0;
	}
    if(cmd->cmd_fd > 0)
        close(cmd->cmd_fd);
	free_commands(&cmd);
	ft_envclear(&data->myenv);
	ft_lstclear(data);
	if (params->fd[0] > -1)
		close(params->fd[0]);
	if (params->fd[1] > -1)
		close(params->fd[1]);
    exit(exit_status);
	
}

void start_child(t_command *cmd, t_data *data, char **envp, t_child_params	*params)
{
	if (!cmd->next && params->is_first)
	{
		if (cmd->fd_type == FD_OUT || cmd->fd_type == APPEND)
         	dup2(cmd->cmd_fd,  STDOUT_FILENO);
		if (cmd->fd_type == FD_IN)
			dup2(cmd->cmd_fd, STDIN_FILENO);
		exec_child(cmd, data, envp, params);
	}
    if (cmd->fd_type == FD_OUT || cmd->fd_type == APPEND)
        dup2(cmd->cmd_fd,  STDOUT_FILENO);
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
	exec_child(cmd, data, envp, params);
}

void	parent_pid(t_command *cmd, t_child_params	*params, t_data *data)
{   int saved_stdout;
    if (cmd->is_bcommand && !cmd->next)
    {
        saved_stdout = dup(STDOUT_FILENO);
        if (cmd->fd_type == FD_OUT || cmd->fd_type == APPEND)
         	dup2(cmd->cmd_fd,  STDOUT_FILENO);
        bcomm_exec(cmd, data);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
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
    if(cmd->cmd_fd > 0)
        close(cmd->cmd_fd);
    params->is_first = 0;
}



void exec_cmd(t_command *cmd, t_data *data, char **envp)
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
        if(!cmd->next && params.is_first && cmd->is_bcommand)
            parent_pid(cmd, &params, data);
        else{
        printf("cmd: %s\n", cmd->command[0]);
	
        pid = fork();
        if (pid == -1)
			return ((perror("fork"), exit(EXIT_FAILURE)));

        if (pid == 0)
			start_child(cmd, data, envp, &params);
		else
			parent_pid(cmd, &params, data);
        }
		cmd = cmd->next;
    }
    if (params.fd_in != STDIN_FILENO)
	{
		close(params.fd_in);
	}
    if (!params.is_first)
	    waitpid(pid, &(data->status), 0);
	printf("status %d\n", data->status);
	printf("exit status: %d\n", WEXITSTATUS(data->status));
    set_exitstatus(data);
	if (data->status== 11)
		printf("Seg fault\n");
}

int open_file(char *filename, int open_type)
{
	int ret_fd;

	ret_fd = 0;
	if (open_type == FD_IN)
	{
		ret_fd = open(filename, O_RDONLY);
		if (ret_fd == -1)
			perror(filename);
	}
	else if (open_type == FD_OUT)
	{
		ret_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (ret_fd == -1)
			perror(filename);
	}
	else if (open_type == APPEND)
	{
		ret_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (ret_fd == -1)
			perror(filename);
	}
	return (ret_fd);
}
