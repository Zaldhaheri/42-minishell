#include "../../inc/minishell.h"

int parse_double_quotes(t_data *data)
{
    data->i++;
    while (data->input[data->i] && data->input[data->i] != '\"')
    {
        append_checker(data);
        data->i++;
        printf(YELLOW "Append DQUOTES: %s\n" RESET, data->checker);
    }
    printf(YELLOW "DQUOTES INPUT CHAR: .%c.\n" RESET, data->input[data->i]);
    data->typeflag = DQUOTES;
    return (1);
}

int parse_single_quotes(t_data *data)
{
    printf(YELLOW "SQUOTES\n");
    data->i++;
    while (data->input[data->i] && data->input[data->i] != '\'')
    {
        append_checker(data);
        data->i++;
        printf(YELLOW "Append SQUOTES: %s\n" RESET, data->checker);
    }
    printf(YELLOW "SQUOTES INPUT CHAR: .%c.\n" RESET, data->input[data->i]);
    data->typeflag = SQUOTES;
    return (1);
}

int parse_in(t_data *data)
{
    printf(BLUE "REDIRECT: %s\n" RESET, data->checker);
    data->checker[ft_strlen(data->checker) - 1] = '\0';
    if (data->i > 0 && ft_strlen(data->checker) != 0 && data->input[data->i - 1] != ' ' && data->input[data->i - 1] != '\t')
        add_token_from_checker(data, WORD, data->checker);
    if (data->input[data->i + 1] == '<')
    {
        data->i++;
        if (data->checker)
            free(data->checker);
        data->checker = ft_strdup("<<");
        add_token_from_checker(data, HEREDOC, data->checker);
    }
    else
    {
        if (data->checker)
            free(data->checker);
        data->checker = ft_strdup("<");
        add_token_from_checker(data, FD_IN, data->checker);
    }
    return (1);
}

int parse_out(t_data *data)
{
    printf(BLUE "REDIRECT: %s\n" RESET, data->checker);
    data->checker[ft_strlen(data->checker) - 1] = '\0';
    if (data->i > 0 && ft_strlen(data->checker) != 0 && data->input[data->i - 1] != ' ' && data->input[data->i - 1] != '\t')
        add_token_from_checker(data, WORD, data->checker);
    if (data->input[data->i + 1] == '>')
    {
        data->i++;
        if (data->checker)
            free(data->checker);
        data->checker = ft_strdup(">>");
        add_token_from_checker(data, APPEND, data->checker);
    }
    else
    {
        if (data->checker)
    free(data->checker);
        data->checker = ft_strdup(">");
        add_token_from_checker(data, FD_OUT, data->checker);
    }
    return (1);
}

int parse_space(t_data *data)
{
    if (ft_strlen(data->checker) == 1 &&
        (ft_strrchr(data->checker, ' ') || ft_strrchr(data->checker, '\t')))
    {
        printf(GREEN "Single space\n" RESET);
        if (data->checker)
            free(data->checker);
        data->checker = ft_strdup("");
        return (0);
    }
    if (ft_strlen(data->input) - 1 > data->i || data->input[data->i] == ' ' ||
        data->input[data->i] == '\t')
    {
        printf("balls\n");
        data->checker[ft_strlen(data->checker) - 1] = '\0';
    }
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
        add_token_from_checker(data, WORD, data->checker);
    }
    ft_lstadd_back(&data->tokens, ft_lstnew("|"));
    return (1);
}
