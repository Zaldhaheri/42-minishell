/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:24:18 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/18 15:27:01 by nalkhate         ###   ########.fr       */
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
t_command *new_command(char **cmd, int fd, int fd_type);
void	create_pipe(t_child_params *params);
void exec_child(t_command *cmd, t_data *data, char **envp, t_child_params	*params);
void start_child(t_command *cmd, t_data *data, char **envp, t_child_params	*params);
void	parent_pid(t_command *cmd, t_child_params	*params, t_data *data);
void exec_cmd(t_command *cmd, t_data *data, char **envp);
int open_file(char *filename, int open_type);
int heredoc(char *limiter);
void bcomm_exec(t_command *cmd, t_data *data);

//builtins
void b_echo(char **com);
void b_env(t_data *data);
void b_cd(t_data *data, char **cmd);
void b_pwd();
#endif