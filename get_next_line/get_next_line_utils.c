/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 19:42:49 by nalkhate          #+#    #+#             */
/*   Updated: 2024/08/20 21:37:26 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*malloc_result(t_list *lst)
{
	int		total_sz;
	char	*res;

	total_sz = 0;
	while (lst != NULL)
	{
		total_sz += ft_strlen(lst->content);
		lst = lst->next;
	}
	res = (char *)malloc((total_sz + 1) * sizeof(char));
	return (res);
}

char	*concatenate_list(t_list **lst)
{
	t_list	*current;
	char	*result;
	char	*temp;

	current = *lst;
	if (!current)
		return (NULL);
	result = malloc_result(current);
	if (!result)
	{
		ft_gnlclear(lst);
		return (NULL);
	}
	result[0] = '\0';
	while (current != NULL)
	{
		temp = ft_strjoin(result, current->content);
		free(result);
		result = temp;
		if (!result)
			return (ft_gnlclear(lst), NULL);
		current = current->next;
	}
	ft_gnlclear(lst);
	return (result);
}
