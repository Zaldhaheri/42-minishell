#include "../inc/minishell.h"


char **exec_command(t_token *curr, char **envp)
{
	t_token *count;
	int		no_of_strings;
	char	**args;
	int		i;
	pid_t	pid;
	int		status;
	no_of_strings = 0;
	count = curr;
	i = 0;
	while(count && count->type != PIPE)
	{
		if (count->type == COMMAND || count->type == FLAG 
		|| count->type == BCOMMAND)
		{
			no_of_strings++;
		}
		count = count->next;
	}
	args = (char **)malloc((no_of_strings + 1) * sizeof(char *) );
	if (!args)
	{
		perror("malloc");
		exit(1);
	}
	while(curr && curr->type != PIPE)
	{
		if (curr->type == COMMAND || curr->type == FLAG 
		|| curr->type == BCOMMAND)
		{
			if (i == 0)
			{
				args[i] = ft_get_cmd_path(curr->content, envp);
			}else
			{
			
				args[i] = ft_strdup(curr->content);
			}
			i++;
		}
		curr = curr->next;
	}
	args[i] = NULL;
	pid = fork();
	if (pid == 0)
	{
		execve(args[0], args, envp);
		exit(0);
		
	}
	waitpid(pid, &status, 0);
	free_args(args);
	return args;
}

void exec_line(t_data *data, char **envp)
{
	char	**exec_arg;
	t_token	*temp;

	(void)exec_arg;
	temp = data->tokens;
	exec_arg = exec_command(temp, envp);
	// while(temp)
	// {
	// 	exec_arg = exec_command(temp, envp);
	// 	temp = temp->next;
	// }

}
