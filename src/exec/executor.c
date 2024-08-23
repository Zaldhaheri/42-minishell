/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:25:03 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/23 16:38:07 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

extern int	g_exit_code;

void	create_pipe(t_child_params *params)
{
	if (pipe(params->fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

int	heredoc(char *limiter)
{
	char	*line;
	int		fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line || ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
			break ;
		write(fd[1], line, ft_strlen(line));
		free(line);
	}
	if (line)
	{
		free(line);
		line = NULL;
	}
	close(fd[1]);
	return (fd[0]);
}

static t_token	*next_cmd(t_command	*command, t_command	**head, t_token *curr)
{
	t_token		*temp;

	temp = curr;
	if (command)
		cmd_add_back(head, command);
	else
		while (temp && temp->type != PIPE)
			temp = temp->next;
	return (temp);
}

void	exec_line(t_data *data)
{
	t_command	*command;
	t_command	*head;
	t_token		*temp;
	char		**cmd;

	temp = data->tokens;
	head = NULL;
	cmd = NULL;
	command = NULL;
	while (temp)
	{
		cmd = cmd_size_init(temp);
		command = set_command(cmd, temp, data, &temp);
		temp = next_cmd(command, &head, temp);
	}
	if (head && *cmd != NULL && command)
		exec_cmd(head, data);
	g_exit_code = 0;
	set_exitstatus(data);
	free_commands(&head);
}

t_token	*handle_pipe(t_token *temp)
{
	if (temp && temp->type == PIPE)
		temp = temp->next;
	return (temp);
}
