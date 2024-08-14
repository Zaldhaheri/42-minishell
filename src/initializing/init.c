#include "../../inc/minishell.h"

void print_env(t_env *env)
{
    t_env *temp;

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

t_env	*ft_envnew(char *word)
{
	t_env *node;
    char **split;
	node = (t_env *)malloc(sizeof(t_env));
    node->key = NULL;
    node->value = NULL;
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

//ex: str: ?=1
void    add_to_myenv(t_data *data, char *str)
{
    if (check_env_dupes(data, str))
        ft_envadd_back(&data->myenv, str);
    if (str)
        free(str);
}

// void    set_exitstatus(t_data *data)
// {
//     //key = ?
//     //value = ft_itoa(data->status)
//     //make it str="key=value"
//     //add_to_myenv(data, str)
// }

void    pre_init(t_data *data, char **envp)
{
    data->envp = envp;
    data->myenv = NULL;
    data->tokens = NULL;
    data->i = 0;
    env_init(data);
    //add_to_myenv(data, "?=0");
    print_env(data->myenv);
}

void    data_init(t_data *data)
{
    data->i = 0;
	data->checker = ft_strdup("");
	data->tokens = NULL;
	data->typeflag = WORD;
}