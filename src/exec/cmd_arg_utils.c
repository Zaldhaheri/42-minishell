/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_arg_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:27:49 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/20 23:24:09 by nalkhate         ###   ########.fr       */
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
	t_command	*temp;

	while (*lst)
	{
		temp = *lst;
		*lst = (*lst)->next;
		if (temp->fd_in > -1)
			close(temp->fd_in);
		if (temp->fd_out > -1)
			close(temp->fd_out);
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
	}
}

int	check_bcommand(char *comm)
{
	if (!comm)
		return (0);
	if (!ft_strncmp(comm, "echo", 4))
		return (1);
	else if (!ft_strncmp(comm, "cd", 2))
		return (1);
	else if (!ft_strncmp(comm, "pwd", 3))
		return (1);
	else if (!ft_strncmp(comm, "export", 6))
		return (1);
	else if (!ft_strncmp(comm, "unset", 5))
		return (1);
	else if (!ft_strncmp(comm, "env", 3))
		return (1);
	else if (!ft_strncmp(comm, "exit", 4))
		return (1);
	else if (ft_strrchr(comm, '=') && ft_strlen(comm) > 1
		&& comm[0] != '=' && is_valid_key(comm))
		return (1);
	else
		return (0);
}

t_command	*new_command(char **cmd, int fd_in, int fd_out, int fd_type)
{
	t_command	*new;

	new = (t_command *)malloc(sizeof(t_command));
	if (!new)
	{
		perror("malloc");
		exit(1);
	}
	new->command = cmd;
	new->next = NULL;
	new->fd_in = fd_in;
	new->fd_out = fd_out;
	new->fd_type = fd_type;
	new->is_bcommand = check_bcommand(cmd[0]);
	return (new);
}
