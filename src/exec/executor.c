/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:25:03 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/11 16:28:16 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "executor.h"


t_command *set_command(char **command,  t_token **temp, char **envp)
{
	int			i;
	int			cmd_fd;
	int			fd_type;
	t_command	*cmd;

	fd_type = NO_FD;
	cmd_fd = NO_FD;
	i = 0;	
	cmd = NULL;
	while (*temp && (*temp)->type != PIPE)
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
	while(temp)
	{
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

