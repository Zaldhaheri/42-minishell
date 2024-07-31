#include "../inc/minishell.h"


typedef struct s_child_params
{
	int	fd[2];
	int	fd_in;
	int	fd_out;
	int	is_first;
}	t_child_params;


void	child_free_exit(t_data *head, t_child_params *params, int isError)
{
	if (isError == 1)
		ft_lstclear(head);
	if (params->fd[0] != -1)
		close(params->fd[0]);
	if (params->fd[1] != -1)
		close(params->fd[1]);
	if (params->fd_in != -1)
		close(params->fd_in);
	if (params->fd_out != -1)
		close(params->fd_out);
	if (isError == 1)
		exit(EXIT_FAILURE);
}

t_command	*cmd_lstlast(t_command *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	cmd_add_back(t_command **lst, t_command *new)
{
	t_command	*temp;

	if (*lst == NULL)
		*lst = new;
	else
	{
		temp = cmd_lstlast(*lst);
		temp->next = new;
		new->prev = temp;
	}
}

t_command *new_command(char **cmd)
{
	t_command *new;

	new = (t_command *)malloc(sizeof(t_command));
	if (!new)
	{
		perror("malloc");
		exit(1);
	}
	new->command = cmd;
	new->next = NULL;
	return(new);
}
void	create_pipe(t_child_params *params)
{
	if (pipe(params->fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}


void	start_child(t_command *temp, t_child_params *params,
	t_data *data, char **envp)
{
	if (temp == NULL || temp->command == NULL)
		child_free_exit(data, params, 1);
	if (params->is_first && params->fd_in == -1)
		child_free_exit(data, params, 1);
	if (temp->next)
	{
		dup2(params->fd[1], STDOUT_FILENO);
	}
	else
	{
		if (params->fd_out == -1)
			child_free_exit(data, params, 1);
		if (params->fd_out > 2)
			dup2(params->fd_out, STDOUT_FILENO);
	}
	close(params->fd[0]);
	close(params->fd[1]);
	//child_free_exit(data, params, 0);
	if (execve(temp->command[0], temp->command, envp) == -1)
	{
		//ft_putstr_fd("command not found: ", STDERR_FILENO);
		perror(temp->command[0]);
		exit(1);
		//child_free_exit(data, params, 1);
	}
}

void	continue_parent(t_child_params *params)
{
	close(params->fd[1]);
	dup2(params->fd[0], STDIN_FILENO);
	close(params->fd[0]);
	params->is_first = 0;
}

void exec_cmd(t_command *cmd, t_data *data, char **envp) {
    pid_t pid;
    int status;
    t_child_params params;
	(void)data;
    params.fd_in = STDIN_FILENO;
    params.fd_out = STDOUT_FILENO;
    params.is_first = 1;

    while (cmd) {
        if (cmd->next) {
            create_pipe(&params);
        }

        printf("cmd: %s\n", cmd->command[0]);
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process
            if (cmd->next) {
                dup2(params.fd[1], STDOUT_FILENO);
            }
            if (!params.is_first) {
                dup2(params.fd_in, STDIN_FILENO);
            }
            close(params.fd[0]);
            close(params.fd[1]);

            if (execve(cmd->command[0], cmd->command, envp) == -1) {
                perror(cmd->command[0]);
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            waitpid(pid, &status, 0); // Wait for the current child process to finish
            if (cmd->next) {
                close(params.fd[1]);
                params.fd_in = params.fd[0];
            }
            params.is_first = 0;
            cmd = cmd->next;
        }
    }

    // Close the remaining open file descriptor in the parent
    if (params.fd_in != STDIN_FILENO) {
        close(params.fd_in);
    }
}

// void	exec_cmd(t_command *cmd, t_data *data, char **envp)
// {
// 	pid_t	pid;
// 	int		status;
// 	t_child_params	params;
	
// 	params.fd_in = STDIN_FILENO;
// 	params.fd_out = STDOUT_FILENO;
// 	params.is_first = 1;
// 	while(cmd)
// 	{
// 		if (cmd->next)
// 			create_pipe(&params);
// 		printf("cmd: %s\n", cmd->command[0]);
// 		pid = fork();
// 		if (pid == -1)
// 			return ((perror("fork"), exit(EXIT_FAILURE)));
// 		if (pid == 0)
// 			start_child(cmd, &params, data, envp);
// 		else
// 			continue_parent(&params);
// 		cmd = cmd->next;
// 	}
// 	//waitpid(pid, &status, 0);
// 	while (wait(&status) > 0);
// 	//ft_lstclear(data);
	
// }

t_command *set_command(char **command,  t_token **temp, char **envp)
{
	int	i;

	i = 0;
	t_command *cmd;
	cmd = NULL;
	(void)envp;
	while(*temp && (*temp)->type != PIPE)
	{
		if ((*temp)->type == COMMAND || (*temp)->type == FLAG 
		|| (*temp)->type == BCOMMAND || (*temp)->type == DQUOTES)
		{
			if (i == 0)
			{
				printf("first cmd: %s\n", (*temp)->content);
				command[i] = ft_get_cmd_path((*temp)->content, envp);
			}else
			{
				printf("flag : %s\n", (*temp)->content);
				command[i] = ft_strdup((*temp)->content);
			}
			i++;
		}
		(*temp) = (*temp)->next;
	}
	if ((*temp) && (*temp)->type == PIPE)
		(*temp) = (*temp)->next;
	command[i] = NULL;
	cmd = new_command(command);
	return (cmd);
}

char	**cmd_size_init(t_token *temp)
{
	t_token		*count;
	size_t		c_len;
	char		**command;

	c_len = 0;
	count = temp;
	while(count && count->type != PIPE)
	{
		if (count->type == COMMAND || count->type == FLAG 
		|| count->type == BCOMMAND)
		{
			c_len++;
		}
		count = count->next;
	}
	command = (char **)malloc((c_len + 1) * sizeof(char *));
	if (!command)
	{
		perror("malloc");
		exit(1);
	}
	return(command);
}

void exec_line(t_data *data, char **envp)
{
	// t_token	*temp;
	t_command *command;
	t_command *head;
	t_token *temp;
	char **cmd;
	temp = data->tokens;
	head = NULL;
	while(temp){
	
	cmd = cmd_size_init(temp);
	command = set_command(cmd, &temp, envp);
	cmd_add_back(&head, command);
	}
	printf("in temp\n");

	if (head && cmd && command)
		exec_cmd(head, data, envp);

	
}

