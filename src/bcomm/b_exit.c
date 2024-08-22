/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:22:34 by zaldhahe          #+#    #+#             */
/*   Updated: 2024/08/22 19:57:36 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec/executor.h"

static void	exit_overflow(char *temp, int *flag)
{
	ft_putstr_fd("exit\nminishell: exit: ", 2);
	ft_putstr_fd(temp, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	*flag = 0;
}

long	ft_atol(const char *str, char *temp, int *flag)
{
	int						sign;
	unsigned long long		result;
	int						digits;

	sign = 1;
	result = 0;
	digits = 1;
	while (*str && (*str == 32 || (*str >= 9 && *str <= 13)))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str && *str >= '0' && *str <= '9')
	{
		result = (result * 10) + *str - 48;
		if (result > 9223372036854775808ULL || digits > 19)
			return (exit_overflow(temp, flag), 255);
		str++;
		if (result > 0)
			digits++;
	}
	return (result * sign);
}

int	is_numeric(char *str)
{
	if (*str == '+' || *str == '-')
		str++;
	while (*str)
	{
		if (*str < 48 || *str > 57)
			return (0);
		str++;
	}
	return (1);
}

void	b_exit_frees(t_data *data, t_command *cmd)
{
	free_commands(&cmd);
	ft_envclear(&data->myenv);
	free_split_from(data->myenvstr, 0);
	ft_lstclear(data);
}

void	b_exit(t_data *data, t_command *cmd, t_child_params	*params)
{
	int	arg;
	int	flag;

	flag = 1;
	arg = 0;
	if (cmd->command[1] && is_numeric(cmd->command[1]) && cmd->command[2])
	{
		printf("exit\n");
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		data->status = 1;
		set_exitstatus(data);
		return ;
	}
	if (cmd->command[1] && is_numeric(cmd->command[1]))
		arg = ft_atol(cmd->command[1], cmd->command[1], &flag);
	else if (cmd->command[1] && !is_numeric(cmd->command[1]))
	{
		exit_overflow(cmd->command[1], &flag);
		arg = 255;
	}
	b_exit_frees(data, cmd);
	if (flag)
		printf("exit\n");
	close(params->saved_stdout);
	return ((close_std(), exit(arg)));
}
