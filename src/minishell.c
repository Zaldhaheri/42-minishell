#include "../inc/minishell.h"

void print_list(t_token *lst)
{
	t_token *temp;

	temp = lst;
	while (temp != NULL)
	{
		if (temp->content)
			printf(RED "%s" RESET, temp->content);
        if (temp->next)
            printf(RED "--" RESET);
		temp = temp->next;;
	}
	printf("\n");
}

void    data_init(t_data *data)
{
    data->i = 0;
	data->checker = ft_strdup(""); //malloc checker to empty
	data->tokens = NULL;
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
        data_init(&data);
		parser(&data);
        //something to clean spaces here
		printf("input: %s\n", data.input);
		ft_lstclear(&data);
	}
	printf("exit\n");
}
