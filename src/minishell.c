#include "../inc/minishell.h"

void print_list(t_token *lst)
{
    t_token *temp;

	temp = lst;
    while (temp != NULL)
    {
		if (temp->content)
			printf(RED "%s " RESET, temp->content);
		temp = temp->next;;
    }
	printf("\n");
}

int check_string(t_data *data)
{
    printf(WHITE "%d: %s\n" RESET, data->i, data->checker);
    //example
    if (ft_strrchr(data->checker, '<') || ft_strrchr(data->checker, '>'))
    {
        printf(BLUE "REDIRECT: %s\n" RESET, data->checker);
        return (1);
    }
    else if (ft_strrchr(data->checker, ' ') || ft_strlen(data->input) - 1 <= data->i)
    {
        ft_lstadd_back(&data->tokens, ft_lstnew(data->checker));
        printf(GREEN "WORD: %s\n" RESET, data->checker);
        print_list(data->tokens);
        return (1);
    }
    return (0);
}

void    parser(t_data *data)
{
    data->i = 0;
    data->checker = ft_strdup(""); //malloc checker to empty
    data->tokens = NULL;
    while(data->input[data->i])
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
        if (check_string(data))
        {
            free(data->checker);
            data->checker = ft_strdup("");
        }
        data->i++;
    }
    free(data->checker);
}

int main()
{
	t_data data;

	while(1)
	{
		data.input = readline("minishell# ");
		if (!data.input)
			break;
		if (data.input[0] != 0)
			add_history(data.input);
        parser(&data);
		printf("input: %s\n", data.input);
	}
	printf("exit\n");
}
