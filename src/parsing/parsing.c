#include "../../inc/minishell.h"

void parser(t_data *data)
{
    while (data->input[data->i])
    {
        if (data->input[data->i] == '\"')
            parse_double_quotes(data);
        else if (data->input[data->i] == '\'')
            parse_single_quotes(data);
        else
        {
            append_checker(data);
            if (check_string(data))
            {
                if (ft_strrchr(data->checker, '='))
                    add_token_to_env(data);
                else
                    add_token_from_checker(data, data->typeflag, &data->checker);
            }
        }
        data->i++;
    }
    add_token_from_checker(data, data->typeflag, &data->checker);
    print_list(data->tokens);
    print_type(data);
    //ft_lstclear(data);
    if (data->checker)
        free(data->checker);
}

void add_token_to_env(t_data *data)
{
    ft_envadd_back(&data->myenv, data->checker);
    if (data->checker)
        free(data->checker);
    data->checker = ft_strdup("");
    print_env(data->myenv);
}

void append_checker(t_data *data)
{
    size_t checker_len = ft_strlen(data->checker);
    size_t new_size = checker_len + 2; // +1 for new char, +1 for null terminator
    data->temp = malloc(new_size);
    if (!data->temp)
    {
        perror("malloc");
        exit(1);
    }
    ft_strcpy(data->temp, data->checker);
    data->temp[checker_len] = data->input[data->i];
    data->temp[checker_len + 1] = 0;
    if (data->checker)
        if (data->checker)
            free(data->checker);
    data->checker = data->temp;
    printf(WHITE "a%d: %s.\n" RESET, data->i, data->checker);
}

void add_token_from_checker(t_data *data, int type, char **str)
{
    t_token *curr;

    if (ft_strlen(*str) > 0)
    {
        printf(BLUE "Adding: %s, Type: %d\n" RESET, *str, type);
        curr = ft_lstnew(ft_strdup(*str));
        curr->type = type;
        ft_lstadd_back(&data->tokens, curr);
        if (*str)
            free(*str);
        *str = ft_strdup("");
        data->typeflag = WORD;
    }
}

int check_string(t_data *data)
{
    if (data->input[data->i] == '<')
        return (parse_in(data));
    else if (data->input[data->i] == '>')
        return (parse_out(data));
    else if (data->input[data->i] == '|')
        return (parse_pipe(data));
    else if (data->input[data->i] == ' ' || data->input[data->i] == '\t' || 
            ft_strlen(data->input) - 1 <= data->i)
        return (parse_space(data));
    return (0);
}
