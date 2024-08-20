/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 20:28:03 by zaldhahe          #+#    #+#             */
/*   Updated: 2024/08/20 20:28:15 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token	*ft_lstnew(char *word)
{
	t_token	*node;

	node = (t_token *) malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->content = ft_strdup(word);
	node->next = NULL;
	node->type = 0;
	return (node);
}

t_token	*ft_lstlast(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_token **lst, t_token *new)
{
	t_token	*temp;

	if (*lst == NULL)
		*lst = new;
	else
	{
		temp = ft_lstlast(*lst);
		temp->next = new;
	}
}

void	ft_lstclear(t_data *lst)
{
	t_token	*temp;

	while (lst->tokens)
	{
		temp = lst->tokens;
		lst->tokens = lst->tokens->next;
		if (temp->content)
			free(temp->content);
		if (temp)
			free(temp);
		temp = NULL;
	}
	lst->tokens = NULL;
}
