#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
# include <unistd.h>

typedef struct s_token
{
	int type;
	int c;
	int	index;
	struct s_token *next;	
} t_token;

typedef struct s_command
{
	char	*command;
	char	**args;
	int		type;
	struct s_command *next;
} t_command;

#endif