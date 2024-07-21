#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
# include <unistd.h>

#define ALPHA 10
#define DIGIT 11
#define WORD 12
#define NUMBER 13
#define COMMAND 14
#define FLAG 15
#define PIPE 16

typedef struct s_token
{
	char	*content;
	int		type;
	int		index;
	struct s_token *next;	
} t_token;

typedef struct s_data
{
	char *input;
	
} t_data;
#endif