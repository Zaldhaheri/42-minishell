/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 21:52:16 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/23 15:33:48 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	open_file(char *filename, int open_type)
{
	int	ret_fd;

	ret_fd = 0;
	if (open_type == FD_IN)
	{
		ret_fd = open(filename, O_RDONLY);
		if (ret_fd == -1)
			perror(filename);
	}
	else if (open_type == FD_OUT)
	{
		ret_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (ret_fd == -1)
			perror(filename);
	}
	else if (open_type == APPEND)
	{
		ret_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (ret_fd == -1)
			perror(filename);
	}
	return (ret_fd);
}
