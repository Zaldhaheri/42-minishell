#include "../includes/minishell.h"

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
		printf("input: %s\n", data.input);
	}
	printf("exit\n");
}

/*
checker(*data)
{
	char *r;
	int i = 0;
	while(data->input[i])
	{
		r += *data->input;
		if (isimportant(r))
			r = 0;
		data->input++;
	}
}
*/