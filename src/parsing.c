#include "../inc/minishell.h"
#include <string.h>

void    append_checker(t_data *data)
{
	data->temp = malloc(ft_strlen(data->checker) + 2); //create temp based on checker size
	if (!data->temp)
	{
		perror("malloc");
		exit(1);
	}
	ft_strcpy(data->temp, data->checker); //create cpy to temp
	data->temp[ft_strlen(data->checker)] = data->input[data->i]; //add character
	data->temp[ft_strlen(data->checker) + 1] = 0; //null terminate it
	free(data->checker);
	data->checker = data->temp;
}

int count_dquotes(t_data *data)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while(data->input[i])
    {
        if (data->input[i] == '\"')
            count++;
        i++;
    }
    return (count);
}

void    parser(t_data *data)
{
	while(data->input[data->i])
	{
        append_checker(data);
		if (check_string(data))
		{
			free(data->checker);
			data->checker = ft_strdup("");
		}
		data->i++;
	}
	free(data->checker);
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
    if (data->input[data->i + 1] == '<') //if '>>'
    {
        data->i++;
        ft_lstadd_back(&data->tokens, ft_lstnew("<<"));
    }
    else
        ft_lstadd_back(&data->tokens, ft_lstnew("<"));
    print_list(data->tokens);
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
    if (data->input[data->i + 1] == '>') //if '>>'
    {
        data->i++;
        ft_lstadd_back(&data->tokens, ft_lstnew(">>"));
    }
    else
        ft_lstadd_back(&data->tokens, ft_lstnew(">"));
    print_list(data->tokens);
	return (1);
}

int parse_space(t_data *data)
{
    if (ft_strlen(data->checker) == 1 &&
        (ft_strrchr(data->checker, ' ') || ft_strrchr(data->checker, '\t')))
        return (1);
    if (ft_strlen(data->input) - 1 > data->i){
        data->checker[ft_strlen(data->checker) - 1] = '\0';}
	ft_lstadd_back(&data->tokens, ft_lstnew(data->checker));
	printf(GREEN "WORD: %s\n" RESET, data->checker);
	print_list(data->tokens);
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
    print_list(data->tokens);
	return (1);
}

int parse_dquotes(t_data *data)
{
    int len;

    t_token *new;

    printf(YELLOW "DQUOTES before: %s\n" RESET, data->checker);
    data->checker[ft_strlen(data->checker) - 1] = '\0';
    if (data->i > 0 && ft_strlen(data->checker) != 0 && data->input[data->i - 1] != ' ' && data->input[data->i - 1] != '\t')
	{
        printf(GREEN "WORD: %s\n" RESET, data->checker);
        ft_lstadd_back(&data->tokens, ft_lstnew(data->checker));
    }
    free(data->checker);
	data->checker = ft_strdup("");
    len = count_dquotes(data);
    printf(YELLOW "DQUOTES LEN: %d\n", len);
    //len--;
    while(len > 0)
    {
        printf(WHITE "%d: %s.\n" RESET, data->i, data->checker);
        if (data->i < ft_strlen(data->input) && data->input[data->i] != '\"') //data->checker[ft_strlen(data->checker) - 1] != '\"'
        {
            append_checker(data);
        }
        else
        {
            printf(YELLOW"len--\n"RESET);
            len--;
        }
        data->i++;
    }
    //data->checker[ft_strlen(data->checker) - 1] = '\0'
    data->i--;
    if (data->checker[0] == '\0')
    {
        print_list(data->tokens);
        return (1);
    }
    printf(YELLOW "DQUOTES after: %s\n" RESET, data->checker);
    new = ft_lstnew(data->checker);
    new->type = DQUOTES;
    ft_lstadd_back(&data->tokens, new);
    print_list(data->tokens);
    return (1);
}

int check_string(t_data *data)
{
	printf(WHITE "%d: %s.\n" RESET, data->i, data->checker);
	//example
	if (ft_strrchr(data->checker, '<'))
		return (parse_in(data));
    else if (ft_strrchr(data->checker, '>'))
        return (parse_out(data));
    else if (ft_strrchr(data->checker, '|'))
        return (parse_pipe(data));
	else if (ft_strrchr(data->checker, ' ') || ft_strrchr(data->checker, '\t') || 
            ft_strlen(data->input) - 1 <= data->i){
        return (parse_space(data));
            }
    else if (ft_strrchr(data->checker, '\"')){
        return (parse_dquotes(data));
    }
	return (0);
}