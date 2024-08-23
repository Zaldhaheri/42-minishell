/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 20:29:53 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/23 16:42:35 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	check_fd_type(t_token *temp)
{
	if (temp->type)
		return ;
	if (temp->next && !ft_strncmp(temp->content, "<", 1)
		&& ft_strlen(temp->content) == 1)
		temp->type = FD_IN;
	else if (temp->next && !ft_strncmp(temp->content, ">", 1)
		&& ft_strlen(temp->content) == 1)
		temp->type = FD_OUT;
	else if (!ft_strncmp(temp->content, ">>", 2))
		temp->type = APPEND;
	else if (!ft_strncmp(temp->content, "<<", 2))
		temp->type = HEREDOC;
}

static int	check_if_command(t_data *data, t_token *curr)
{
	t_token	*temp;
	int		command_flag;

	if (curr->type)
		return (2);
	command_flag = 0;
	temp = data->tokens;
	while (temp && temp != curr)
	{
		if (temp->type == COMMAND || temp->type == BCOMMAND
			|| temp->type == FILENAME || temp->type == LIMITER)
			command_flag = 1;
		else if (temp->type == PIPE)
			command_flag = 0;
		else if (temp->type == FD_IN || temp->type == FD_OUT
			|| temp->type == APPEND)
			command_flag = 3;
		else if (temp->type == HEREDOC)
			command_flag = 4;
		temp = temp->next;
	}
	return (command_flag);
}

void	check_if_bcommand(t_token *temp)
{
	if (temp->type)
		return ;
	if (!ft_strncmp(temp->content, "echo", 4))
		temp->type = BCOMMAND;
	else if (!ft_strncmp(temp->content, "cd", 2))
		temp->type = BCOMMAND;
	else if (!ft_strncmp(temp->content, "pwd", 3))
		temp->type = BCOMMAND;
	else if (!ft_strncmp(temp->content, "export", 6))
		temp->type = BCOMMAND;
	else if (!ft_strncmp(temp->content, "unset", 5))
		temp->type = BCOMMAND;
	else if (!ft_strncmp(temp->content, "env", 3))
		temp->type = BCOMMAND;
	else if (!ft_strncmp(temp->content, "exit", 4))
		temp->type = BCOMMAND;
	else if (ft_strrchr(temp->content, '=') && ft_strlen(temp->content) > 1
		&& temp->content[0] != '=' && is_valid_key(temp->content))
		temp->type = BCOMMAND;
}

static void	token_type(t_token *temp, t_data *data)
{
	if (!ft_strncmp(temp->content, "|", 1) && ft_strlen(temp->content) == 1)
		temp->type = PIPE;
	else if (check_if_command(data, temp) == 1)
		temp->type = FLAG;
	else if (check_if_command(data, temp) == 0)
		temp->type = COMMAND;
	else if (check_if_command(data, temp) == 3)
		temp->type = FILENAME;
	else if (check_if_command(data, temp) == 4)
		temp->type = LIMITER;
}

void	set_type(t_data *data)
{
	t_token	*temp;

	if (!data || !data->tokens)
		return ;
	temp = data->tokens;
	while (temp)
	{
		if (temp->type == DOLLAR)
			temp->type = WORD;
		if (!temp->type)
		{
			check_fd_type(temp);
			check_if_bcommand(temp);
			token_type(temp, data);
		}
		temp = temp->next;
	}
	set_filename(data->tokens);
}
