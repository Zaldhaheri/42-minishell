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

int main(int argc, char **argv, char **envp)
{
	t_data data;

	(void)argc;
	(void)argv;
	while(1)
	{
		data.input = readline("minishell# ");
		if (!data.input)
			break;
		if (data.input[0] != 0)
		{
			printf("input: %s\n", data.input);
			add_history(data.input);
        	data_init(&data, envp);
			parser(&data);
			//set_type(&data);
			//exec_line(&data, envp);
			ft_lstclear(&data);
		}
	}
	printf("exit\n");
}
