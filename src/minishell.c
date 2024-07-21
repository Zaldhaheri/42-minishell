#include "../includes/minishell.h"

int main()
{
	char    *str;

	str = readline("minishell# ");
	while(str)
	{
		if (*str)
        {
            printf("%s", str);
			add_history(str);
        }
        str = readline("minishell# ");
		//parse(str)
		//execute(result)
	}	
	return (0);
}