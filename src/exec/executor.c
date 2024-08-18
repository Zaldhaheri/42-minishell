/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:25:03 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/18 19:01:16 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "executor.h"

int heredoc(char *limiter)
{
    printf("heredoc limiter: %s\n", limiter);
    char *line;
    int fd[2];
    
    if (pipe(fd) == -1)
    {
        perror("pipe");
        return -1;
    }
    while (1)
    {
        write(1, "> ", 2);
        line = get_next_line(0);
        if (!line || ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
            break;
        write(fd[1], line, ft_strlen(line));
        free(line);
    }
    if (line) {
   		free(line);
    	line = NULL;
	}
    close(fd[1]);
    return (fd[0]);
}

t_command *set_command(char **command,  t_token *temp, char **envp, t_token **head)
{
	int			i;
	int			cmd_fd;
	int			fd_type;
	t_command	*cmd;

	fd_type = NO_FD;
	cmd_fd = NO_FD;
	i = 0;	
	cmd = NULL;
	while (temp  && temp->type != PIPE)
	{
		if (temp->type == COMMAND || temp->type == FLAG 
		|| temp->type == BCOMMAND || temp->type == DQUOTES || temp->type == SQUOTES)
		{
			if (i == 0 && temp->type != BCOMMAND )
			{
				printf("first cmd: %s\n", temp->content);
				command[i] = ft_get_cmd_path(temp->content, envp);
				if (temp->content && !command[i])
					command[i] = ft_strdup(temp->content);
			}else if (i == 0 && temp->type == BCOMMAND)
			{
				command[i] = ft_strdup(temp->content);
			}
			else
			{
				printf("flag : %s\n", temp->content);
				command[i] = ft_strdup(temp->content);
			}
			i++;
		}
		else if (temp->type == FD_IN || temp->type == FD_OUT 
		|| temp->type == APPEND || temp->type == HEREDOC)
		{
			if (temp->next && temp->type != HEREDOC){
				cmd_fd = open_file(temp->next->content, temp->type);
				*head = temp;
				if  (cmd_fd == -1 && i > 0)
				{
					command[i] = NULL;
					free_args(command);
					return (NULL);
				}
				else if (cmd_fd == -1)
					return (NULL);
				
			}
			else if (temp->next && temp->type == HEREDOC)
			{
				cmd_fd = heredoc(temp->next->content);
				temp->type = FD_IN;
			}
			else if(!temp->next)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				if  (i > 0)
				{
					command[i] = NULL;
					free_args(command);
				}
				*head = temp;
				return (NULL);
			}
			fd_type = temp->type;
		}

		temp = temp->next;
	}
	if (temp && temp->type == PIPE)
		temp = temp->next;
	*head = temp;
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
		if (temp->type == COMMAND || temp->type == FLAG 
		|| temp->type == BCOMMAND || temp->type == DQUOTES || temp->type == SQUOTES)
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
	while(temp)
	{
		cmd = cmd_size_init(temp);
		command = set_command(cmd, temp, envp, &temp);
		if (command)
			cmd_add_back(&head, command);
		else
			while(temp && temp->type != PIPE)
				temp = temp->next;
	}
	if (head && *cmd != NULL && command)
		exec_cmd(head, data, envp);
	free_commands(&head);
}

