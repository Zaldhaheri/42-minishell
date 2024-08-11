/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_arg_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:27:49 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/11 16:28:49 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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
