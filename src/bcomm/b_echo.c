/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:05:00 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/20 18:06:03 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../exec/executor.h"

void b_pwd()
{
    char *path;

    path = NULL;
    path = getcwd(NULL, PATH_MAX);
    if (!path){
        perror("pwd");
    }
    else{
        printf("%s\n", path);
        free(path);
    }
}

void	process_echo_arg(char **com, int *i, int *flag, int *not_words)
{
	char	*temp;
	int		j;

	temp = com[*i];
	if (!ft_strncmp(temp, "-n", 2) && *not_words == 1)
	{
		j = 1;
		while (temp[j] == 'n')
			j++;
		if (temp[j])
		{
			*not_words = 0;
			printf("%s", temp);
			if (com[*i + 1])
				printf(" ");
		}
		else
			*flag = 1;
	}
	else
	{
		*not_words = 0;
		printf("%s", temp);
		if (com[*i + 1])
			printf(" ");
	}
}

void	init_echo_vars(int *flag, int *i, int *not_words)
{
	*flag = 0;
	*i = 1;
	*not_words = 1;
}

void	b_echo(char **com)
{
	int	flag;
	int	i;
	int	not_words;

	init_echo_vars(&flag, &i, &not_words);
	while (com[i] != NULL)
	{
		process_echo_arg(com, &i, &flag, &not_words);
		i++;
	}
	if (!flag)
		printf("\n");
}
