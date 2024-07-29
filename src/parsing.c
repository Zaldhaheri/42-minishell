#include "../inc/minishell.h"

// int count_dquotes(t_data *data)
// {
//     int i;
//     int count;

//     i = 0;
//     count = 0;
//     while (data->input[i])
//     {
//         if (data->input[i] == '\"')
//             count++;
//         i++;
//     }
//     return (count);
// }

void parser(t_data *data)
{
    while (data->input[data->i])
    {
        if (data->input[data->i] == '\"')
        {
            parse_double_quotes(data);
        }
        else
        {
            append_checker(data);
            if (check_string(data))
            {
                print_list(data->tokens);
                free(data->checker);
                data->checker = ft_strdup("");
            }
            data->i++;
        }
    }
    add_token_from_checker(data);
    free(data->checker);
}

void append_checker(t_data *data)
{
    data->temp = malloc(ft_strlen(data->checker) + 2);
    if (!data->temp)
    {
        perror("malloc");
        exit(1);
    }
    ft_strcpy(data->temp, data->checker);
    data->temp[ft_strlen(data->checker)] = data->input[data->i];
    data->temp[ft_strlen(data->checker) + 1] = 0;
    free(data->checker);
    data->checker = data->temp;
}

void add_token_from_checker(t_data *data)
{
    if (ft_strlen(data->checker) > 0)
    {
        ft_lstadd_back(&data->tokens, ft_lstnew(ft_strdup(data->checker)));
        free(data->checker);
        data->checker = ft_strdup("");
    }
}

int check_string(t_data *data)
{
    printf(WHITE "%d: %s.\n" RESET, data->i, data->checker);
    if (ft_strrchr(data->checker, '<'))
        return (parse_in(data));
    else if (ft_strrchr(data->checker, '>'))
        return (parse_out(data));
    else if (ft_strrchr(data->checker, '|'))
        return (parse_pipe(data));
    else if (data->input[data->i] == ' ' || data->input[data->i] == '\t' || 
            ft_strlen(data->input) - 1 <= data->i)
        return (parse_space(data));
    return (0);
}
