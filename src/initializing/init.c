#include "../../inc/minishell.h"

void free_split_from(char **split, int from)
{
    while(split[from])
    {
        free(split[from]);
        from++;
    }
    free(split);
}

void	env_init(t_data *data)
{
    if (data->myenv)
        return ;
    data->myenv = NULL;
    while(data->envp[data->i])
    {
        ft_envadd_back(&data->myenv, data->envp[data->i], 0);
        data->i++;
    }
    data->i = 0;
}

void    add_to_myenv(t_data *data, char *str, int hide)
{
    if (check_env_dupes(data, str))
        ft_envadd_back(&data->myenv, str, hide);
    if (str)
        free(str);
}

void    pre_init(t_data *data, char **envp)
{
    data->envp = envp;
    data->myenv = NULL;
    data->tokens = NULL;
    data->i = 0;
    env_init(data);
    add_to_myenv(data, ft_strdup("?=0"), 1);
    incr_shlvl(data);
    set_pwd(data);
    print_env(data->myenv);
}

void    data_init(t_data *data)
{
    data->i = 0;
	data->checker = ft_strdup("");
	data->tokens = NULL;
	data->typeflag = WORD;
    data->myenvstr = env_to_array(data->myenv);
}

int ft_envsize(t_env *myenv)
{
    t_env *curr;
    int i;

    i = 0;
    curr = myenv;
    while(curr)
    {
        if (curr->key && curr->value && !curr->hide)
            i++;
        curr = curr->next;
    }
    return (i);
}

char **env_to_array(t_env *myenv)
{
    printf("envtoarray\n");
    int size;
    char *temp;
    char **array;
    t_env *curr;
    int i;

    i = 0;
    size = ft_envsize(myenv);
    if (size == 0)
        return NULL;
    array = malloc((size + 1) * sizeof(char *));
    curr = myenv;
    while(i < size)
    {
        if (curr->key && curr->value && !curr->hide)
        {
            temp = ft_strjoin(curr->key, "=");
            array[i] = ft_strjoin(temp, curr->value);
            free(temp);
            i++;
        }
        curr = curr->next;
    }
    array[size] = NULL;
    return array;
}
