/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:24:22 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/16 20:04:12 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "executor.h"
#include <errno.h>


void exec_child(t_command *cmd, t_data *data, char **envp, int *fd)
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
		bcomm_exec(cmd);
		exit_status = 0;
	}
	free_commands(&cmd);
	ft_envclear(&data->myenv);
	ft_lstclear(data);
	if (fd[0] > -1)
		close(fd[0]);
	if (fd[1] > -1)
		close(fd[1]);
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
		exec_child(cmd, data, envp, params->fd);
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
	exec_child(cmd, data, envp, params->fd);
}

void	parent_pid(t_command *cmd, t_child_params	*params)
{
	if (cmd->next)
	{
        close(params->fd[1]);
		if (cmd->fd_type == FD_IN)
			params->fd_in = cmd->cmd_fd;
		else
           	params->fd_in = params->fd[0];
    }
    params->is_first = 0;
}

//bcommands execute here
void bcomm_exec(t_command *cmd)
{
	if (!ft_strncmp("echo", cmd->command[0], 4))
		b_echo(cmd->command);
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
        printf("cmd: %s\n", cmd->command[0]);
	
        pid = fork();
        if (pid == -1)
			return ((perror("fork"), exit(EXIT_FAILURE)));

        if (pid == 0)
			start_child(cmd, data, envp, &params);
		else
			parent_pid(cmd, &params);
		
		cmd = cmd->next;
    }
    if (params.fd_in != STDIN_FILENO)
	{
		close(params.fd_in);
	}
	waitpid(pid, &(data->status), 0);
	printf("status %d\n", data->status);
	printf("exit status: %d\n", WEXITSTATUS(data->status));
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
