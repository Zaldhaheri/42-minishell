
#include "../exec/executor.h"

void b_echo(char **com)
{
    int flag;
    int i = 1;
    flag = 0;
    char *temp;
    int j;
    int not_words = 1;
   ft_putstr_fd("BCOMM--->echo function\n",2);
    while(com[i] != NULL)
    {
        temp = com[i];
        if (!ft_strncmp(temp, "-n", 2) && not_words == 1)
        {
            j = 1;
            while(temp && temp[j] == 'n')
            {
                j++;
            }
            if (temp[j])
            {
                not_words = 0;
                printf("%s", temp);
                if (com[i + 1])
                    printf(" ");
            }
            else{
                flag = 1;
            }

        }else
        {
            not_words = 0;
            printf("%s", temp);
            if (com[i + 1])
                printf(" ");
        }
        
        i++;
    }
    if (!flag)
        printf("\n");
}