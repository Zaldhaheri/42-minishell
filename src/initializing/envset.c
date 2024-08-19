#include "../../inc/minishell.h"

void    set_exitstatus(t_data *data)
{
    char *status_str;
    char *exit_status;
    char *temp;

    status_str = ft_itoa(data->status);
    temp = ft_strjoin("?", "=");
    exit_status = ft_strjoin(temp, status_str);
    free(status_str);
    free(temp);
    add_to_myenv(data, exit_status, 1, 0);
}

void set_pwd(t_data *data)
{
    char *str;
    char *finalstr;
    char *temp;

    str = getcwd(NULL, PATH_MAX);
    temp = ft_strjoin("PWD", "=");
    finalstr = ft_strjoin(temp, str);
    free(str);
    free(temp);
    add_to_myenv(data, finalstr, 0, 0);
}

void incr_shlvl(t_data *data)
{
    char *shlvl_str;
    char *new_shlvl_str;
    int shlvl;
    char *value;

    value = get_env_value(data, "SHLVL");
    if (!value)
        shlvl = 1;
    else
        shlvl = ft_atoi(value) + 1;
    shlvl_str = ft_itoa(shlvl);
    new_shlvl_str = ft_strjoin("SHLVL=", shlvl_str);
    add_to_myenv(data, new_shlvl_str, 0, 0);
    if (shlvl_str)
        free(shlvl_str);
}