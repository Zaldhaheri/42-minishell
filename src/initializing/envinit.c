#include "../../inc/minishell.h"

void print_env(t_env *env)
{
	t_env *temp;

	temp = env;
	while(temp)
	{
		if (temp->key && temp->value && !temp->hide)
		{
			printf(GREEN "%s" RESET, temp->key);
			printf("=");
			printf(BLUE "%s\n" RESET, temp->value);
		}
		temp = temp->next;
	}
	printf("\n");
}

t_env	*ft_envlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void ft_envclear(t_env **lst)
{
	t_env *curr;
	t_env *temp;

	curr = *lst;
	while (curr)
	{
		temp = curr;
		if (curr->key && curr->value)
		{
			free(curr->value);
			free(curr->key);
		}
		curr = curr->next;
		if (temp)
		{
			free(temp); 
		}
	}
	*lst = NULL; 
}

t_env	*ft_envnew(char *word, int hide)
{
	t_env *node;
	char **split;
	node = (t_env *)malloc(sizeof(t_env));
	node->key = NULL;
	node->value = NULL;
	node->hide = hide;
	if (!node)
		return (NULL);
	split = ft_split(word, '=');
	if (split[0] && split[1])
	{
		node->key = ft_strdup(split[0]);
		node->value = ft_strdup(split[1]);
	}
	node->next = NULL;
	free_split_from(split, 0);
	return (node);
}

void    ft_envadd_back(t_env **lst, char *s, int hide)
{
	t_env *temp;

	if (*lst == NULL)
		*lst = ft_envnew(s, hide);
	else
	{
		temp = ft_envlast(*lst);
		temp->next = ft_envnew(s, hide);
	}
}
