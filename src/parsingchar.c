#include "../inc/minishell.h"

int parse_double_quotes(t_data *data)
{
    data->i++;
    while (data->input[data->i] && data->input[data->i] != '\"')
    {
        append_checker(data);
        data->i++;
    }
    if (data->input[data->i] == '\"')
        data->i++; 
    return (1);
}


int parse_in(t_data *data)
{
    printf(BLUE "REDIRECT: %s\n" RESET, data->checker);
    data->checker[ft_strlen(data->checker) - 1] = '\0';
    if (data->i > 0 && ft_strlen(data->checker) != 0 && data->input[data->i - 1] != ' ' && data->input[data->i - 1] != '\t')
    {
        printf(GREEN "WORD: %s\n" RESET, data->checker);
        ft_lstadd_back(&data->tokens, ft_lstnew(data->checker));
    }
    if (data->input[data->i + 1] == '<')
    {
        data->i++;
        ft_lstadd_back(&data->tokens, ft_lstnew("<<"));
    }
    else
        ft_lstadd_back(&data->tokens, ft_lstnew("<"));
    return (1);
}

int parse_out(t_data *data)
{
    printf(BLUE "REDIRECT: %s\n" RESET, data->checker);
    data->checker[ft_strlen(data->checker) - 1] = '\0';
    if (data->i > 0 && ft_strlen(data->checker) != 0 && data->input[data->i - 1] != ' ' && data->input[data->i - 1] != '\t')
    {
        printf(GREEN "WORD: %s\n" RESET, data->checker);
        ft_lstadd_back(&data->tokens, ft_lstnew(data->checker));
    }
    if (data->input[data->i + 1] == '>')
    {
        data->i++;
        ft_lstadd_back(&data->tokens, ft_lstnew(">>"));
    }
    else
        ft_lstadd_back(&data->tokens, ft_lstnew(">"));
    return (1);
}

int parse_space(t_data *data)
{
    if (ft_strlen(data->checker) == 1 &&
        (ft_strrchr(data->checker, ' ') || ft_strrchr(data->checker, '\t')))
        return (1);
    if (ft_strlen(data->input) - 1 > data->i || data->input[data->i] == ' ' ||
        data->input[data->i] == '\t'){
        data->checker[ft_strlen(data->checker) - 1] = '\0';}
    ft_lstadd_back(&data->tokens, ft_lstnew(data->checker));
    printf(GREEN "WORD: %s.\n" RESET, data->checker);
    return (1);
}

int parse_pipe(t_data *data)
{
    printf(CYAN "PIPE: %s\n" RESET, data->checker);
    data->checker[ft_strlen(data->checker) - 1] = '\0';
    if (data->i > 0 && ft_strlen(data->checker) != 0 && data->input[data->i - 1] != ' ' && data->input[data->i - 1] != '\t')
    {
        printf(GREEN "WORD: %s\n" RESET, data->checker);
        ft_lstadd_back(&data->tokens, ft_lstnew(data->checker));
    }
    ft_lstadd_back(&data->tokens, ft_lstnew("|"));
    return (1);
}
