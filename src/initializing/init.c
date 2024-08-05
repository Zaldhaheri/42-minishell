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

void    data_init(t_data *data, char **envp)
{
    data->i = 0;
	data->checker = ft_strdup(""); //malloc checker to empty
	data->tokens = NULL;
	data->typeflag = WORD; //set type flag to WORD
	data->envp = envp;
	//data->myenv = NULL;
    env_init(data);
    print_env(data->myenv);
}