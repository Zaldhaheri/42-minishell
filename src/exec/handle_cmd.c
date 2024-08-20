/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:30:08 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/20 16:30:11 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	handle_command(t_token *temp, char **command, t_data *data, t_cmd_data *cmd_data)
{
	if (cmd_data->i == 0 && temp->type != BCOMMAND)
	{
		command[cmd_data->i] = ft_get_cmd_path(temp->content, data->myenvstr);
	}
	else if (cmd_data->i == 0 && (!command[cmd_data->i] || temp->type == BCOMMAND))
		command[cmd_data->i] = ft_strdup(temp->content);
	else
		command[cmd_data->i] = ft_strdup(temp->content);
	cmd_data->i++;
}

int	handle_redirection(t_token *temp, char **command, t_data_bundle *bundle)
{
	if (temp->next && temp->type != HEREDOC)
	{
		if (bundle->cmd_data->cmd_fd > -1)
			close(bundle->cmd_data->cmd_fd);
		bundle->cmd_data->cmd_fd = open_file(temp->next->content, temp->type);
		*(bundle->head) = temp;
		if (!validate_fd(bundle->cmd_data->cmd_fd, bundle->cmd_data->i, command))
			return (0);
	}
	else if (temp->next && temp->type == HEREDOC)
	{
		bundle->cmd_data->cmd_fd = heredoc(temp->next->content);
		temp->type = FD_IN;
	}
	else if (!temp->next)
	{
		handle_syntax_error(bundle->cmd_data->i, command, bundle->data, bundle->head, temp);
		return (0);
	}
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

void	handle_syntax_error(int i, char **command, t_data *data, t_token **head, t_token *temp)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
	data->status = 258;
	if (i > 0)
	{
		command[i] = NULL;
		free_args(command);
	}
	*head = temp;
}

t_token	*handle_pipe(t_token *temp)
{
	if (temp && temp->type == PIPE)
		temp = temp->next;
	return (temp);
}
