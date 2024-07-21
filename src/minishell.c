#include "../includes/minishell.h"

int main()
{
    char *str = readline("minishell# ");
    printf("%s\n", str);
}