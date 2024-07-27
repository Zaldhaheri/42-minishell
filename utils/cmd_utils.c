/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:00:43 by nalkhate          #+#    #+#             */
/*   Updated: 2024/07/27 23:42:19 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

static	char	*ft_get_cmd_path_from_path(char *cmd, char **path)
{
	int		i;
	char	*cmd_path;

	i = 0;
	while (path[i])
	{
		cmd_path = ft_strjoin(path[i], cmd);
		if (!cmd_path)
			return (NULL);
		if (access(cmd_path, F_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (ft_strdup(cmd));
}

static	char	**ft_strjoin_arr(char **arr, char *str)
{
	int		i;
	char	**new_arr;
	char	*temp;

	i = 0;
	while (arr[i])
		i++;
	new_arr = malloc(sizeof(char *) * (i + 1));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		temp = arr[i];
		new_arr[i] = ft_strjoin(arr[i], str);
		free(temp);
		i++;
	}
	new_arr[i] = NULL;
	i = 0;
	free(arr);
	return (new_arr);
}

char	*ft_get_cmd_path(char *cmd, char **envp)
{
	int		i;
	char	**path;
	char	*cmd_path;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_split(envp[i] + 5, ':');
			if (!path)
				return (NULL);
			path = ft_strjoin_arr(path, "/");
			if (!path)
				return (NULL);
			cmd_path = ft_get_cmd_path_from_path(cmd, path);
			free_args(path);
			return (cmd_path);
		}
		i++;
	}
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	int		i;

	i = 0;
	if (!s1 || !s2)
		return (NULL);
	joined = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!joined)
		return (NULL);
	while (*s1)
		joined[i++] = *s1++;
	while (*s2)
		joined[i++] = *s2++;
	joined[i] = '\0';
	return (joined);
}
