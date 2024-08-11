/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:24:18 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/11 19:19:37 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "../../inc/minishell.h"

typedef struct s_child_params
{
	int	fd[2];
	int	fd_in;
	int	fd_out;
	int	is_first;
}	t_child_params;

t_command	*cmd_lstlast(t_command *lst);
void	free_commands(t_command **lst);
void	cmd_add_back(t_command **lst, t_command *new);
t_command *new_command(char **cmd, int fd, int fd_type, char *limiter);
void	create_pipe(t_child_params *params);
void exec_child(t_command *cmd, t_data *data, char **envp);
void start_child(t_command *cmd, t_data *data, char **envp, t_child_params	*params);
void	parent_pid(t_command *cmd, t_child_params	*params);
void exec_cmd(t_command *cmd, t_data *data, char **envp);
int open_file(char *filename, int open_type);
int heredoc(char *limiter);

#endif