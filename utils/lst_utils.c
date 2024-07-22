#include "../inc/minishell.h"

t_token	*ft_lstnew(char *word)
{
	t_token *node;
	node = (t_token *)malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->content = word;
	node->next = NULL;
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

void	ft_lstclear(t_token **lst)
{
	t_token	*temp;

	if (*lst)
	{
		while (*lst != NULL)
		{
			temp = *lst;
			*lst = (*lst)->next;
			free(temp->content);
			free(temp);
		}
		*lst = NULL;
	}
}
