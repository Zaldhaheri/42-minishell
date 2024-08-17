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
    print_env(data->myenv);
}

void    data_init(t_data *data)
{
    data->i = 0;
	data->checker = ft_strdup("");
	data->tokens = NULL;
	data->typeflag = WORD;
}