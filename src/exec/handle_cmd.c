/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:30:08 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/23 17:05:06 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	handle_command(t_token *temp, char **command,
	t_data *data, t_cmd_data *cmd_data)
{
	if (cmd_data->i == 0 && temp->type != BCOMMAND)
	{
		command[cmd_data->i] = ft_get_cmd_path(temp->content, data->myenvstr);
		if (!command[cmd_data->i])
			command[cmd_data->i] = ft_strdup(temp->content);
	}
	else if (cmd_data->i == 0 && (!command[cmd_data->i]
			|| temp->type == BCOMMAND))
		command[cmd_data->i] = ft_strdup(temp->content);
	else
		command[cmd_data->i] = ft_strdup(temp->content);
	cmd_data->i++;
}

void	read_in_out(t_token *temp, t_data_bundle *bundle)
{
	if (temp->type == FD_OUT || temp->type == APPEND)
	{
		if (bundle->cmd_data->fd_out > -1)
			close(bundle->cmd_data->fd_out);
		bundle->cmd_data->fd_out = open_file(temp->next->content, temp->type);
		if (bundle->cmd_data->fd_out == -1)
		{
			bundle->data->status = 1;
			set_exitstatus(bundle->data);
		}
		*(bundle->head) = temp;
	}
	else if (temp->type == FD_IN)
	{
		if (bundle->cmd_data->fd_in > -1)
			close(bundle->cmd_data->fd_in);
		bundle->cmd_data->fd_in = open_file(temp->next->content, temp->type);
		if (bundle->cmd_data->fd_in == -1)
		{
			bundle->data->status = 1;
			set_exitstatus(bundle->data);
		}
		*(bundle->head) = temp;
	}
}

int	handle_redirection(t_token *temp, char **command, t_data_bundle *bundle)
{
	if (temp->next && temp->type != HEREDOC)
	{
		read_in_out(temp, bundle);
		if (temp->type == FD_IN && !validate_fd(bundle->cmd_data->fd_in,
				bundle->cmd_data->i, command))
			return (0);
		else if (temp->type == FD_OUT && !validate_fd(bundle->cmd_data->fd_out,
				bundle->cmd_data->i, command))
			return (0);
	}
	else if (temp->next && temp->type == HEREDOC)
	{
		bundle->cmd_data->fd_in = heredoc(temp->next->content);
		temp->type = FD_IN;
	}
	else if (!temp->next)
	{
		handle_syntax_error(bundle, command, temp);
		return (0);
	}
	if (bundle->cmd_data->fd_in > -1 && bundle->cmd_data->fd_out > -1)
		bundle->cmd_data->fd_type = BOTH_FD;
	else
		bundle->cmd_data->fd_type = temp->type;
	return (1);
}

int	validate_fd(int cmd_fd, int i, char **command)
{
	if (cmd_fd == -1 && i > 0)
	{
		command[i] = NULL;
		free_args(command);
		return (0);
	}
	else if (cmd_fd == -1)
		return (0);
	return (1);
}

void	handle_syntax_error(t_data_bundle *bundle,
	char **command, t_token *temp)
{
	ft_putstr_fd
		("minishell: syntax error near unexpected token `newline'\n", 2);
	bundle->data->status = 258;
	if (bundle->cmd_data->i > 0)
	{
		command[bundle->cmd_data->i] = NULL;
		free_args(command);
	}
	bundle->head = &temp;
}
