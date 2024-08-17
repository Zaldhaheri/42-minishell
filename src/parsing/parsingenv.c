#include "../../inc/minishell.h"

int is_valid_key(char *str)
{
    int i;

    i = 0;
    while(str[i] && str[i] != '=')
    {
        if (ft_isalnum(str[i]) || str[i] == '_')
        {
            i++;
        }
        else
        {
            return (0);
        }
    }
    return (1);
}

int check_env_dupes(t_data *data, char *str)
{
    int flag;
    char **split;
    t_env *curr;

    curr = data->myenv;
    flag = 1;
    split = ft_split(str, '=');
    data->temp = split[0];
    //printf("temp: .%s.\n", data->temp);
    while(curr)
    {
        if (curr->key && !ft_strcmp(data->temp, curr->key))
        {
            //printf("key found\n");
            free(curr->value);
            curr->value = ft_strdup(split[1]);
            flag = 0;
            break ;
        }
        curr = curr->next;
    }
    if (split)
        free_split_from(split, 0);
    return (flag);
}

void add_token_to_env(t_data *data)
{
    printf(RED "is env\n" RESET);
    if (check_env_dupes(data, data->checker))
        ft_envadd_back(&data->myenv, data->checker, 1);
    if (data->checker)
        free(data->checker);
    data->checker = ft_strdup("");
    print_env(data->myenv);
}

char *get_env_value(t_data *data, char *key)
{
    t_env *curr;

    curr = data->myenv;
    if (!key)
        return(NULL);
    while(curr)
    {
        if (curr->key && !ft_strcmp(key, curr->key))
            return (curr->value);
        curr = curr->next;
    }
    return (NULL);
}