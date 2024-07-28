#include "../inc/minishell.h"

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

t_command *new_command()
{
	t_command *new;

	new = (t_command *)malloc(sizeof(t_command));
	if (!new)
	{
		perror("malloc");
		exit(1);
	}
	new->c_len = 0;
	new->command = NULL;
	new->next = NULL;
	return(new);
}
void	create_pipe(int *fd)
{
	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

void	exec_cmd(t_command *cmd, t_data *data, char **envp)
{
	pid_t	pid;
	int		status;
	int		fd[2];
	if (cmd->next)
	{
		create_pipe(fd);
	}
	
	while(cmd)
	{
		printf("cmd: %s\n", cmd->command[0]);
		pid = fork();
		if (pid == 0)
		{
			printf("inside fork\n");
			execve(cmd->command[0], cmd->command, envp);
			perror(cmd->command[0]);
			ft_lstclear(data);
			exit(1);
		}
		cmd = cmd->next;
	}
	waitpid(pid, &status, 0);
	
}

t_token	*set_command(t_command *cmd, t_token *curr, char **envp)
{
	int	i;

	i = 0;
	t_token *temp;

	temp = curr;
	while(temp && temp->type != PIPE)
	{
		if (temp->type == COMMAND || temp->type == FLAG 
		|| temp->type == BCOMMAND)
		{
			if (i == 0)
			{
				cmd->command[i] = ft_get_cmd_path(temp->content, envp);
			}else
			{
				cmd->command[i] = ft_strdup(temp->content);
			}
			i++;
		}
		temp = temp->next;
	}
	return (temp);
}

void	create_command(t_token *tokens, t_command *cmd)
{
	t_token		*count;

	count = tokens;
	while(count && count->type != PIPE)
	{
		if (count->type == COMMAND || count->type == FLAG 
		|| count->type == BCOMMAND)
		{
			cmd->c_len++;
		}
		count = count->next;
	}
	cmd->command = (char **)malloc((cmd->c_len + 1) * sizeof(char *));
	if (!cmd->command)
	{
		perror("malloc");
		exit(1);
	}
}

// t_token *exec_command(t_data *data, t_token *curr, char **envp)
// {
// 	t_token *count;
// 	int		no_of_strings;
// 	char	**args;
// 	int		i;
// 	pid_t	pid;
// 	int		status;
// 	no_of_strings = 0;
// 	count = curr;
// 	i = 0;
// 	while(count && count->type != PIPE)
// 	{
// 		if (count->type == COMMAND || count->type == FLAG 
// 		|| count->type == BCOMMAND)
// 		{
// 			no_of_strings++;
// 		}
// 		count = count->next;
// 	}
// 	args = (char **)malloc((no_of_strings + 1) * sizeof(char *));
// 	if (!args)
// 	{
// 		perror("malloc");
// 		exit(1);
// 	}
// 	while(curr && curr->type != PIPE)
// 	{
// 		if (curr->type == COMMAND || curr->type == FLAG 
// 		|| curr->type == BCOMMAND)
// 		{
// 			if (i == 0)
// 			{
// 				args[i] = ft_get_cmd_path(curr->content, envp);
// 			}else
// 			{
			
// 				args[i] = ft_strdup(curr->content);
// 			}
// 			i++;
// 		}
// 		curr = curr->next;
// 	}
// 	args[i] = NULL;
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		printf("inside fork\n");
// 		execve(args[0], args, envp);
// 		perror(args[0]);
// 		free_args(args);
// 		ft_lstclear(data);
// 		exit(1);
		
// 	}
// 	waitpid(pid, &status, 0);
// 	free_args(args);
// 	return curr;
// }

void exec_line(t_data *data, char **envp)
{
	t_token	*temp;
	t_command *command;
	t_command *head;
	
	head = NULL;
	temp = data->tokens;
	(void)envp;
	// while(temp){
		
		command = new_command();
		create_command(temp, command);
		temp = set_command(command, temp, envp);
		cmd_add_back(&head, command);
		command = NULL;
	// }
	exec_cmd(head, data, envp);


}
