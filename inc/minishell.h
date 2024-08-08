#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
# include <fcntl.h> 

// #define WORD 10
#define WORD 0
#define COMMAND 11 //execve
#define FD_IN 12
#define FD_OUT 13
#define APPEND 14
#define HEREDOC 15
#define FILENAME 16
#define LIMITER 17
#define PIPE 18
#define DQUOTES 19
#define SQUOTES 20
#define DOLLAR 21
#define DECLARE 22
#define BCOMMAND 23 //builtin
#define FLAG 24
#define NO_FD -10

#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

typedef struct s_token t_token;
typedef struct s_env t_env;

struct s_env
{
	char *content; //A=1
	char *key; //A
	char *value; //1
	struct s_env *next;
};

struct s_token
{
	char	*content;
	int		type;
	struct s_token *next;
};

typedef struct s_data
{
	char *input;
	char *checker; //the appender
	char *temp; //temp string
	int typeflag;
	char **envp; //original envp
	t_env	*myenv; //our env
	t_token *tokens;
	t_token *currtoken;
	unsigned int i;
} t_data;

typedef struct s_command
{
	char **command;
	int cmd_fd;
	int fd_type;
	struct s_command *next;
	struct s_command *prev;
} t_command;

void print_list(t_token *lst);
void print_type(t_data *data);
void print_env(t_env *env);

//initializing
void    data_init(t_data *data);
void    pre_init(t_data *data, char **envp);
void	env_init(t_data *data);
t_env	*ft_envlast(t_env *lst);
t_env	*ft_envnew(char *word);
void    ft_envadd_back(t_env **lst, char *s);

//parsing
void parser(t_data *data);
void append_checker(t_data *data);
int check_string(t_data *data);
void add_token_from_checker(t_data *data, int type, char **str);
void add_token_to_env(t_data *data);
int parse_double_quotes(t_data *data);
int parse_single_quotes(t_data *data);
int parse_in(t_data *data);
int parse_out(t_data *data);
int parse_space(t_data *data);
int parse_pipe(t_data *data);

void	set_type(t_data *data);

//utils
size_t	ft_strlen(const char *str);
char	*ft_strdup(const char *str);
void ft_strcpy(char *dst, const char *src);
int	ft_strcmp(const char *s1, const char *s2);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strrchr(const char *str, int c);
char	*ft_strtrim(char const *s1, char const *set);
size_t	ft_strlcpy(char *dest, const char *src, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);

//lst utils
t_token	*ft_lstnew(char *word);
t_token	*ft_lstlast(t_token *lst);
void	ft_lstadd_back(t_token **lst, t_token *new);
void	ft_lstclear(t_data *lst);

//exec
void exec_line(t_data *data, char **envp);
void	free_args(char **args);
char	*ft_get_cmd_path(char *cmd, char **envp);

#endif