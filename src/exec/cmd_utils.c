/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:32:06 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/23 16:46:46 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	is_valid_type(int type)
{
	if (type == COMMAND || type == FLAG
		|| type == BCOMMAND || type == DQUOTES
		|| type == SQUOTES)
	{
		return (1);
	}
	else if (type == FD_IN || type == FD_OUT
		|| type == APPEND || type == HEREDOC)
	{
		return (2);
	}
	return (0);
}

t_command	*set_command(char **command, t_token *temp,
	t_data *data, t_token **head)
{
	t_cmd_data		cmd_data;
	t_data_bundle	bundle;

	init_cmd_data(&cmd_data);
	bundle.data = data;
	bundle.head = head;
	bundle.cmd_data = &cmd_data;
	while (temp && temp->type != PIPE)
	{
		if (!handle_token_type(temp, command, &bundle))
		{
			free_args(command);
			return (NULL);
		}
		temp = temp->next;
	}
	*head = handle_pipe(temp);
	command[cmd_data.i] = NULL;
	return (new_command(command, cmd_data.fd_in,
			cmd_data.fd_out, cmd_data.fd_type));
}

int	handle_token_type(t_token *temp, char **command, t_data_bundle *bundle)
{
	if (is_valid_type(temp->type) == 1)
		handle_command(temp, command, bundle->data, bundle->cmd_data);
	else if (is_valid_type(temp->type) == 2)
	{
		if (!handle_redirection(temp, command, bundle))
			return (0);
	}
	return (1);
}

void	init_cmd_data(t_cmd_data *cmd_data)
{
	cmd_data->fd_in = NO_FD;
	cmd_data->fd_out = NO_FD;
	cmd_data->fd_type = NO_FD;
	cmd_data->i = 0;
}

char	**cmd_size_init(t_token *temp)
{
	t_token	*count;
	size_t	c_len;
	char	**command;
	size_t	i;

	i = 0;
	c_len = 0;
	count = temp;
	command = NULL;
	while (count && count->type != PIPE)
	{
		if (count->type == COMMAND || count->type == FLAG
			|| count->type == BCOMMAND || count->type == DQUOTES
			|| count->type == SQUOTES)
		{
			c_len++;
		}
		count = count->next;
	}
	command = (char **)malloc((c_len + 1) * sizeof(char *));
	while (i <= c_len)
	{
		command[i++] = NULL;
	}
	return (command);
}
