#include "../includes/minishell.h"

int main()
{
	char    *str;

	str = readline("minishell# ");
	while(str)
	{
		if (*str)
			add_history(str);
		str = readline("minishell# ");
		printf("%s\n", );
		//parse(str)
		//execute(result)
	}
	
	return (0);
}