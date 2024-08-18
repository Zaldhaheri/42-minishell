

// void exec_cmd(t_command *cmd, t_data *data, char **envp)
// {
//     pid_t			pid;
//     t_child_params	params;

//     params.fd_in = STDIN_FILENO;
//     params.fd_out = STDOUT_FILENO;
//     params.is_first = 1;
// 	params.fd[0] = -10;
// 	params.fd[1] = -10;
//     while (cmd)
// 	{
//         if (cmd->next)
//             create_pipe(&params);
//         printf("cmd: %s\n", cmd->command[0]);
// 		if (!(cmd->is_bcommand && params.is_first))
//         	pid = fork();
//         if (pid == -1)
// 			return ((perror("fork"), exit(EXIT_FAILURE)));

//         if (pid == 0)
// 			start_child(cmd, data, envp, &params);
// 		else
// 			parent_pid(cmd, &params, data);
// 		params.is_first = 0;
// 		cmd = cmd->next;
//     }
//     if (params.fd_in != STDIN_FILENO)
// 	{
// 		close(params.fd_in);
// 	}
// 	printf("waiting...\n");
// 	waitpid(pid, &(data->status), 0);
// 	printf("status %d\n", data->status);
// 	printf("exit status: %d\n", WEXITSTATUS(data->status));
// 	set_exitstatus(data);
// 	if (data->status== 11)
// 		printf("Seg fault\n");
// }

void exec_child(t_command *cmd, t_data *data, char **envp, t_child_params *params)
{
    int exit_status = 1;

    // Execute the command if it's not a built-in command
    if (cmd && cmd->command[0] && !cmd->is_bcommand)
    {
        execve(cmd->command[0], cmd->command, envp);
        // If execve fails, print an error message
        perror(cmd->command[0]);
    }

    // Handle built-in commands in the child process if necessary
    if (cmd->is_bcommand && !params->is_first)
    {
        ft_putstr_fd("-child--BCOMM-----\n", 2);
        bcomm_exec(cmd, data);
        exit_status = 0;
    }

    // Clean up resources
    free_commands(&cmd);
    ft_envclear(&data->myenv);
    ft_lstclear(data);

    // Close pipes in the child process
    if (params->fd[0] > -1)
    {
        close(params->fd[0]);
    }
    if (params->fd[1] > -1)
    {
        close(params->fd[1]);
    }

    exit(exit_status);
}

void parent_pid(t_command *cmd, t_child_params *params, t_data *data)
{
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout < 0)
    {
        perror("dup");
        return;
    }

    if (params->is_first && cmd->is_bcommand)
    {
        // Handle output redirection for the built-in command
        if (cmd->fd_type == FD_OUT || cmd->fd_type == APPEND)
        {
            dup2(cmd->cmd_fd, STDOUT_FILENO);
        }
        else if (cmd->next)
        {
            dup2(params->fd[1], STDOUT_FILENO); // Write to the pipe
        }

        // Handle input redirection for the built-in command
        if (cmd->fd_type == FD_IN)
        {
            dup2(cmd->cmd_fd, STDIN_FILENO);
        }

        // Execute the built-in command
        bcomm_exec(cmd, data);

        // Close the write end of the pipe after writing to it
        close(params->fd[1]);

        // Restore stdout in the parent process
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);

        return; // Exit after handling the built-in command in the parent
    }

    if (cmd->next)
    {
        close(params->fd[1]); // Close the write end in the parent
        if (cmd->fd_type == FD_IN)
        {
            params->fd_in = cmd->cmd_fd;
        }
        else
        {
            params->fd_in = params->fd[0]; // Save the read end for the next command
        }
    }else
        dup2(saved_stdout, STDOUT_FILENO);
    ft_putstr_fd("reached end of parent\n", 2);
    close(saved_stdout);
    params->is_first = 0; // Mark that the first command has been handled
}

void start_child(t_command *cmd, t_data *data, char **envp, t_child_params *params)
{
    ft_putstr_fd("in child\n", 2);

    // Redirect stdout for output redirection or pipes
    if (cmd->fd_type == FD_OUT || cmd->fd_type == APPEND)
    {
        dup2(cmd->cmd_fd, STDOUT_FILENO);
    }
    else if (cmd->next)
    {
        dup2(params->fd[1], STDOUT_FILENO); // Redirect stdout to the pipe
    }

    ft_putstr_fd("child: after user fds\n", 2);

    // Redirect stdin for input redirection or pipes
    if (!params->is_first)
    {
        ft_putstr_fd("child: not first\n", 2);
        if (cmd->fd_type == FD_IN)
        {
            dup2(cmd->cmd_fd, STDIN_FILENO);
        }
        else
        {
            dup2(params->fd_in, STDIN_FILENO); // Redirect stdin from the pipe
        }
    }

    // Close the write end of the pipe in the child process
    close(params->fd[1]);

    // Execute the command in the child process
    exec_child(cmd, data, envp, params);
}
