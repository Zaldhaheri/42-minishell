/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 17:57:48 by zaldhahe          #+#    #+#             */
/*   Updated: 2024/08/21 17:57:48 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_exit_code;

void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	minishell(t_data *data)
{
	add_history(data->input);
	data_init(data);
	parser(data);
	set_type(data);
	exec_line(data);
	free_split_from(data->myenvstr, 0);
	ft_lstclear(data);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	(void)argv;
	g_exit_code = 0;
	pre_init(&data, envp);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signal_handler);
	while (1)
	{
		data.input = readline(MAGENTA "minishell# " RESET);
		if (!data.input)
			break ;
		if (data.input[0] != 0)
			minishell(&data);
		else
		{
			data.status = g_exit_code;
		}
	}
	ft_envclear(&data.myenv);
	ft_lstclear(&data);
	printf("exit\n");
}
