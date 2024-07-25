#ifndef TESTSHELL_H
#define TESTSHELL_H

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
	char	*cmd_path;
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


size_t	ft_strlen(const char *str);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s1);char	*ft_strdup(const char *s1);
void 	ft_strcpy(char *dst, const char *src);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strrchr(const char *str, int c);
char	**ft_split(char const *s, char c);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
t_token	*ft_lstnew(char *word);
void	ft_lstclear(t_token **lst);
void	ft_lstadd_back(t_token **lst, t_token *new);
void	free_args(char **args);
char	*ft_get_cmd_path(char *cmd, char **envp);
char	*ft_strjoin(char const *s1, char const *s2);

#endif