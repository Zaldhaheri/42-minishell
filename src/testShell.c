#include "testShell.h"

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

void print_args(t_args *args)
{
	t_args *temp;

	temp = args;
	printf("args: ");
    while (temp)
    {
		printf("%s,", temp->content);
        temp = temp->next;
    }
	printf("\n");
}

void print_list(t_token *lst)
{
    t_token *temp;

	temp = lst;
    while (temp != NULL)
    {
		if (temp->content)
			printf("COMMAND: %s\n", temp->content);
		if (temp->arg)
			print_args(temp->arg);
        if (temp->next == NULL)
			break ;
		temp = temp->next;
		printf("\n");
    }
	printf("\n");
}


void parser(t_data *data, char **envp)
{
	t_token *cmd;
	t_args	*c_arg;
	char	**splitted;
	char	*no_flag;
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
	cmd->next = NULL;
	cmd->arg = ft_new_arg(ft_strdup(splitted[i]));;
	i++;
	while(splitted[i])
	{
		c_arg = ft_new_arg(ft_strdup(splitted[i]));
		ft_argadd_back(&(cmd->arg), c_arg);
		c_arg = NULL;
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
	printf("DEBUG\n");
	char *temp;
	temp = ft_get_cmd_path(cmd->arg->content, envp);
	if (temp)
		printf("cmd_path: %s\n", temp);
	//ft_lstclear(&cmd);
}

int main(int argc, char **argv, char **envp)
{
	t_data data;
	(void)argc;
	(void)argv;
	(void)envp;

	while(1)
	{
		data.input = readline("minishell# ");
		if(!data.input)
			break;
		if (data.input[0] != 0)
		{
			add_history(data.input);
			parser(&data, envp);
		}
		printf("input: %s\n", data.input);
	}
	printf("exit\n");
}
// To compile (from repo root)
// cc src/testShell.c utils/ft_split.c  utils/utils1.c utils/lst_utils.c -Wall -Werror -Wextra -lreadline ; ./a.out

// The Idea is to have a Node for each commands, and its arguments Since execve takes the command, command + args 
// e.g. ls -a -l --> execve("/bin/ls", {"/bin/ls", "-a", "-l"})