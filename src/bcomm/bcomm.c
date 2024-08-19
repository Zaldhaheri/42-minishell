
#include "../exec/executor.h"

void b_echo(char **com)
{
    int flag;
    int i = 1;
    flag = 0;
    char *temp;
    int j;
    int not_words = 1;
   ft_putstr_fd("BCOMM--->echo function\n",2);
    while(com[i] != NULL)
    {
        temp = com[i];
        if (!ft_strncmp(temp, "-n", 2) && not_words == 1)
        {
            j = 1;
            while(temp && temp[j] == 'n')
            {
                j++;
            }
            if (temp[j])
            {
                not_words = 0;
                printf("%s", temp);
                if (com[i + 1])
                    printf(" ");
            }
            else{
                flag = 1;
            }

        }else
        {
            not_words = 0;
            printf("%s", temp);
            if (com[i + 1])
                printf(" ");
        }
        
        i++;
    }
    if (!flag)
        printf("\n");
}

void b_env(t_data *data)
{
    t_env *curr;

    curr = data->myenv;
    printf("My env bcomm\n");
    while (curr)
    {
        if (curr->key && curr->value && !curr->hide)
            printf("%s=%s\n", curr->key, curr->value);
        curr = curr->next;
    }
}

void b_sort(char **arr, int size)
{
    int swap;
    char *temp;
    int i;

    swap = 1;
    while(swap)
    {
        i = 0;
        swap = 0;
        while(i < size - 1)
        {
            if (ft_strcmp(arr[i], arr[i + 1]) > 0)
            {
                temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                swap = 1;
            }
            i++;
        }
    }
}

void print_export(t_data *data)
{
    char **tempenv;
    int i;

    i = 0;
    tempenv = env_to_array(data->myenv);
    b_sort(tempenv, ft_envsize(data->myenv));
    while(tempenv[i])
    {
        printf("%s\n", tempenv[i++]);
    }
    free_split_from(tempenv, 0);
}

void b_export(t_data *data, char **cmd)
{
    int i;

    i = 0;
    printf("export\n");
    if (!cmd[1])
        print_export(data);
    else
    {
        while(cmd[i])
        {
            if (ft_strrchr(cmd[i], '=') && ft_strlen(cmd[i]) > 1
                    && cmd[i][0] != '=' && is_valid_key(cmd[i]))
                add_to_myenv(data, ft_strdup(cmd[i]), 0);
            i++;
        }
    }
}

void b_unset(t_data *data, char **cmd)
{
    t_env *curr;
    t_env *prev;
    t_env *temp;
    int i;

    i = 1;
    while(cmd[i])
    {
        curr = data->myenv;
        prev = NULL;
        while(curr)
        {
            if (curr->key && !ft_strcmp(curr->key, cmd[i]) && curr->key[0] != '?')
            {
                printf("key %s found\n", curr->key);
                prev->next = curr->next;
                temp = curr;
                curr = curr->next;
                free(temp->key);
                free(temp->value);
                free(temp);
                break ;
            }
            prev = curr;
            curr = curr->next;
        }
        i++;
    }
}

void b_cd(t_data *data, char **cmd)
{
    char *home;

    home = get_env_value(data, "HOME");
    if (!cmd[1] || !ft_strncmp(cmd[1], "~", 1)){
        if (!home)
        {
            ft_putstr_fd("home directory is not set.\n", 2);
        }
        else
            chdir(home);
    }
    else if (chdir(cmd[1]) == -1){
        perror("cd");
        data->status = 1;
    }
}

void b_pwd()
{
    char *path;

    path = getcwd(NULL, PATH_MAX);
    if (!path)
        perror("pwd");
    else{
        printf("%s\n", path);
        free(path);
    }

}

static void	exit_overflow(char *temp, int *flag)
{
	ft_putstr_fd("exit\nminishell: exit: ", 2);
    ft_putstr_fd(temp, 2);
    ft_putstr_fd(": numeric argument required\n", 2);
    *flag = 0;
}

long	ft_atol(const char *str, char *temp, int *flag)
{
	int						sign;
	unsigned long long		result;
	int						digits;

	sign = 1;
	result = 0;
	digits = 1;
	while (*str && (*str == 32 || (*str >= 9 && *str <= 13)))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str && *str >= '0' && *str <= '9')
	{
		result = (result * 10) + *str - 48;
		if (result > 9223372036854775808ULL || digits > 19)
			return (exit_overflow(temp, flag), 255);
		str++;
		if (result > 0)
			digits++;
	}
	return (result * sign);
}

int is_numeric(char *str)
{
    if (*str == '+' || *str == '-')
        str++;
    while(*str)
    {
        if (*str < 48 || *str > 57)
            return 0;
        str++;
    }
    return 1;
}

void    b_exit(t_data *data, t_command *cmd)
{
    int arg;
    int flag;

    flag = 1;
    arg = 0;
    if (cmd->command[1] && is_numeric(cmd->command[1]) &&cmd->command[2])
    {
        printf("exit\n");
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
        data->status = 1;
        set_exitstatus(data);
        return ;
    }
    if (cmd->command[1] && is_numeric(cmd->command[1]))
        arg = ft_atol(cmd->command[1], cmd->command[1], &flag);
    else if (cmd->command[1] && !is_numeric(cmd->command[1]))
    {
        exit_overflow(cmd->command[1], &flag);
        arg = 255;
    }
    free_commands(&cmd);
    ft_envclear(&data->myenv);
    free_split_from(data->myenvstr, 0);
	ft_lstclear(data);
    if (flag)
        printf("exit\n");
    exit(arg);
}


