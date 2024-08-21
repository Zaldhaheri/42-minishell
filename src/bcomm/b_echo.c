/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:05:00 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/21 12:43:14 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec/executor.h"

char *get_pwd()
{
	char	*path;

	path = NULL;
	path = getcwd(NULL, PATH_MAX);
	if (!path)
		return("");
	else
		return (path);
}

void	b_pwd(void)
{
	char	*path;

	path = NULL;
	path = getcwd(NULL, PATH_MAX);
	if (!path)
		perror("pwd");
	else
	{
		printf("%s\n", path);
		free(path);
	}
}

void	process_echo_arg(char **com, t_data *data, int *flag, int *not_words)
{
	char	*temp;

	temp = com[data->i];
	if (!ft_strncmp(temp, "-n", 2) && *not_words == 1)
	{
		data->j = 1;
		while (temp[data->j] == 'n')
			data->j++;
		if (temp[data->j])
		{
			*not_words = 0;
			printf("%s", temp);
			if (com[data->i + 1])
				printf(" ");
		}
		else
			*flag = 1;
	}
	else
	{
		*not_words = 0;
		printf("%s", temp);
		if (com[data->i + 1])
			printf(" ");
	}
}

void	b_echo(t_data *data, char **com)
{
	int	flag;
	int	not_words;

	flag = 0;
	data->i = 1;
	not_words = 1;
	while (com[data->i] != NULL)
	{
		process_echo_arg(com, data, &flag, &not_words);
		data->i++;
	}
	if (!flag)
		printf("\n");
}
