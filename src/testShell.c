#include <stdio.h>
#include <stdlib.h>
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

#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

typedef struct s_args
{
	char *content;
	struct s_args *next;
} t_args;

typedef struct s_token
{
	char	*content;
	int		type;
	int		index;
	t_args	*arg;
	struct s_token *next;
} t_token;

typedef struct s_data
{
	char *input;
	char *checker; //the appender
	char *temp; //temp string
	unsigned int i;
} t_data;

t_args	*ft_new_arg(char *word)
{
	t_args *node;
	node = (t_args *)malloc(sizeof(t_args));
	if (!node)
		return (NULL);
	node->content = word;
	node->next = NULL;
	return(node);
}

t_args	*ft_arglast(t_args *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_argadd_back(t_args **lst, t_args *new)
{
	t_args	*temp;

	if (*lst == NULL)
		*lst = new;
	else
	{
		temp = ft_arglast(*lst);
		temp->next = new;
	}
}

size_t	ft_strlen(const char *str);
char	*ft_strdup(const char *str);
void 	ft_strcpy(char *dst, const char *src);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strrchr(const char *str, int c);
char	**ft_split(char const *s, char c);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
t_token	*ft_lstnew(char *word);
void	ft_lstclear(t_token **lst);
void	ft_lstadd_back(t_token **lst, t_token *new);

int	flag_index(char *line)
{
	//the ide for flag is to break the line where we need it to break
	int i = 0;
	while (line[i])
	{
		if (line[i] == '<' || line[i] == '>' || line[i] == '$')
		{
			return i;
		}
		i++;
	}
	return(i + 1);
	
}
void print_list(t_token *lst)
{
	t_args *args;
	while(lst)
	{
		if(lst->arg)
			args = lst->arg;
		if (lst->content)
			printf(BLUE "command: %s\n" RESET,lst->content);
		printf(CYAN "command + arguments: " RESET);
		while(lst->arg && args)
		{
			printf("%s, ", args->content);
			args =  args->next;
		}
		printf("\n");
		lst = lst->next;
	}
}


void    parser(t_data *data)
{
	t_token *cmd;
	t_args	*c_arg;
	char **splitted;
	char *no_flag;
	int		i;
	int		flag;

	i = 0;
	flag = flag_index(data->input);
	printf(RED "flag at index %d\n" RESET, flag);
	no_flag = malloc(flag + 2);
	ft_strlcpy(no_flag, data->input, flag);
	printf(GREEN "No flag: %s\n" RESET, no_flag);
	splitted = ft_split(no_flag, ' ');
	cmd = ft_lstnew(ft_strdup(splitted[i]));
	cmd->arg = ft_new_arg(ft_strdup(splitted[i]));;
	i++;
	while(splitted[i])
	{
		c_arg = ft_new_arg(ft_strdup(splitted[i]));
		ft_argadd_back(&(cmd->arg), c_arg);
		i++;
	}
	i = 0;
	while(splitted[i])
	{
		free(splitted[i++]);
	}
	free(splitted);
	free(no_flag);
	print_list(cmd);
	ft_lstclear(&cmd);
}

int main()
{
	t_data data;

	while(1)
	{
		data.input = readline("minishell# ");
		if(!data.input)
			break;
		if (data.input[0] != 0)
		{
			add_history(data.input);
			parser(&data);
		}
		printf("input: %s\n", data.input);
	}
	printf("exit\n");
}
// To compile (from repo root)
// cc src/testShell.c utils/ft_split.c  utils/utils1.c utils/lst_utils.c -Wall -Werror -Wextra -lreadline ; ./a.out

// The Idea is to have a Node for each commands, and its arguments Since execve takes the command, command + args 
// e.g. ls -a -l --> execve("/bin/ls", {"/bin/ls", "-a", "-l"})