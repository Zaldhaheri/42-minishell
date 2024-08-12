#include "../../inc/minishell.h"

void print_env(t_env *env)
{
    t_env *temp;

	temp = env;
	while (temp != NULL)
	{
		if (temp->content)
			printf(CYAN "%s\n" RESET, temp->content);
		temp = temp->next;
	}
    temp = env;
    while(temp)
    {
        if (temp->key && temp->value)
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

//free split index from and up
void free_split_from(char **split, int from)
{
    while(split[from])
    {
        free(split[from]);
        from++;
    }
    free(split);
}

void ft_envclear(t_env **lst)
{
    t_env *curr;
    t_env *temp;

    curr = *lst;
    while (curr)
    {
        temp = curr;
        free(curr->key);
        free(curr->value);
        curr = curr->next;
        free(temp); 
    }
    *lst = NULL; 
}

t_env	*ft_envnew(char *word)
{
	t_env *node;
    char **split;
	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	split = ft_split(word, '=');
    node->content = word;
    node->key = split[0];
    node->value = split[1];
	node->next = NULL;
    free_split_from(split, 2);
	return (node);
}

// //place string like A=1 to add to env list
void    ft_envadd_back(t_env **lst, char *s)
{
    t_env *temp;

    if (*lst == NULL)
        *lst = ft_envnew(s);
    else
    {
        temp = ft_envlast(*lst);
        temp->next = ft_envnew(s);
    }
}

void	env_init(t_data *data)
{
    if (data->myenv)
        return ;
    data->myenv = NULL;
    while(data->envp[data->i])
    {
        ft_envadd_back(&data->myenv, data->envp[data->i]);
        data->i++;
    }
    data->i = 0;
}

void    pre_init(t_data *data, char **envp)
{
    data->envp = envp;
    data->myenv = NULL;
    data->i = 0;
    env_init(data);
    print_env(data->myenv);
}

void    data_init(t_data *data)
{
    data->i = 0;
	data->checker = ft_strdup("");
	data->tokens = NULL;
	data->typeflag = WORD;
}