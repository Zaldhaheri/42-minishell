#include "../inc/minishell.h"


typedef struct s_child_params
{
	int	fd[2];
	int	fd_in;
	int	fd_out;
	int	is_first;
}	t_child_params;

t_command	*cmd_lstlast(t_command *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	free_commands(t_command **lst)
{
	t_command *temp;

	while(*lst)
	{
		temp = *lst;
		*lst = (*lst)->next;
		if (temp->cmd_fd > -1)
		{
			close(temp->cmd_fd);
		}
		free_args(temp->command);
		free(temp);
	}
	*lst = NULL;
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

t_command *new_command(char **cmd, int fd, int fd_type)
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
	new->cmd_fd = fd;
	new->fd_type = fd_type;
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
			if (!cmd->next && params.is_first)
			{
				if (cmd->fd_type == FD_OUT || cmd->fd_type == APPEND)
                	dup2(cmd->cmd_fd,  STDOUT_FILENO);
				if (cmd->fd_type == FD_IN)
					dup2(cmd->cmd_fd, STDIN_FILENO);
				if (execve(cmd->command[0], cmd->command, envp) == -1) {
                	perror(cmd->command[0]);
					free_commands(&cmd);
					ft_lstclear(data);
                	exit(EXIT_FAILURE);
            }
			}
            if (cmd->fd_type == FD_OUT || cmd->fd_type == APPEND)
                	dup2(cmd->cmd_fd,  STDOUT_FILENO);
            else if (cmd->next) {	
				dup2(params.fd[1], STDOUT_FILENO);
            }
            if (!params.is_first) {
				if (cmd->fd_type == FD_IN)
					dup2(cmd->cmd_fd, STDIN_FILENO);
                else
					dup2(params.fd_in, STDIN_FILENO);
            }
            if (cmd->next) {
                close(params.fd[0]);
            }
            close(params.fd[1]);


            if (execve(cmd->command[0], cmd->command, envp) == -1) {
                perror(cmd->command[0]);
				free_commands(&cmd);
				ft_lstclear(data);
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
             // Wait for the current child process to finish
            if (cmd->next) {
                close(params.fd[1]);
				if (cmd->fd_type == FD_IN)
					params.fd_in = cmd->cmd_fd;
				else
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
	waitpid(pid, &status, 0);
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

t_command *set_command(char **command,  t_token **temp, char **envp)
{
	int	i;
	int cmd_fd;
	int fd_type;

	fd_type = NO_FD;
	cmd_fd = NO_FD;
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
		else if ((*temp)->type == FD_IN || (*temp)->type == FD_OUT 
		|| (*temp)->type == APPEND)
		{
			if ((*temp)->next)
				cmd_fd = open_file((*temp)->next->content, (*temp)->type);
			fd_type = (*temp)->type;
		}
		(*temp) = (*temp)->next;
	}
	if ((*temp) && (*temp)->type == PIPE)
		(*temp) = (*temp)->next;
	command[i] = NULL;
	cmd = new_command(command, cmd_fd, fd_type);
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
	t_command *command;
	t_command *head;
	t_token *temp;
	char **cmd;
	temp = data->tokens;
	head = NULL;
	while(temp){
	
		cmd = cmd_size_init(temp);
		command = set_command(cmd, &temp, envp);
		printf("hello my fire d\n");
		cmd_add_back(&head, command);
	}
	printf("in temp\n");

	if (head && *cmd != NULL && command)
		exec_cmd(head, data, envp);

	free_commands(&head);
}

