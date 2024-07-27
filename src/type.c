#include "../inc/minishell.h"

static void	check_fd_type(t_token *temp)
{
	if (temp->type)
		return ;
	if (temp->next && !ft_strncmp(temp->content,"<",1) && ft_strlen(temp->content) == 1)
	{
		temp->type = FD_IN;
		temp->next->type = FILENAME;
	}
	else if(temp->next && !ft_strncmp(temp->content,">",1) && ft_strlen(temp->content) == 1)
	{
		temp->type = FD_OUT;
		temp->next->type = FILENAME;
	}
	else if(!ft_strncmp(temp->content,">>",2))
	{
		temp->type = APPEND;
		temp->next->type = FILENAME;
	}
	else if(!ft_strncmp(temp->content,"<<",2))
	{
		temp->type = HEREDOC;
		temp->next->type = LIMITER;
	}
}

static int	check_if_command(t_data *data, t_token *curr)
{
	t_token	*temp;
	int		command_flag;
	// int		pipe_flag;

	if (curr->type)
		return 2;
	command_flag = 0;
	// pipe_flag = 0;
	temp = data->tokens;
	while(temp && temp != curr)
	{
		if (temp->type == COMMAND || temp->type == BCOMMAND)
			command_flag = 1;
		else if(temp->type == PIPE)
		{
			// pipe_flag = 1;
			command_flag = 0;
		}
		temp = temp->next;
	}
	return(command_flag);

}

void print_type(t_data *data)
{
	t_token	*temp;

	temp = data->tokens;
	while(temp)
	{
		printf(MAGENTA"NODE: %s Type: %d " RESET, temp->content, temp->type);
		temp = temp->next;
	}
	printf("\n");
}
void	check_if_bcommand(t_token *temp)
{
	int	c_len;

	if (temp->type)
		return ;
	c_len = ft_strlen(temp->content);
	if (!ft_strncmp(temp->content, "echo", c_len))
		temp->type = BCOMMAND;
	else if (!ft_strncmp(temp->content, "cd", c_len))
		temp->type = BCOMMAND;
	else if (!ft_strncmp(temp->content, "cd", c_len))
		temp->type = BCOMMAND;
	else if (!ft_strncmp(temp->content, "pwd", c_len))
		temp->type = BCOMMAND;
	else if (!ft_strncmp(temp->content, "export", c_len))
		temp->type = BCOMMAND;
	else if (!ft_strncmp(temp->content, "unset", c_len))
		temp->type = BCOMMAND;
	else if (!ft_strncmp(temp->content, "env", c_len))
		temp->type = BCOMMAND;
	else if (!ft_strncmp(temp->content, "exit", c_len))
		temp->type = BCOMMAND;
}

void	set_type(t_data *data)
{
	t_token	*temp;

	temp = data->tokens;
	while(temp)
	{
		if (!temp->type)
		{
			check_fd_type(temp);
			check_if_bcommand(temp);
			if (!ft_strncmp(temp->content,"|",1) && ft_strlen(temp->content) == 1)
				temp->type = PIPE;
			else if (check_if_command(data, temp) == 1)
				temp->type = FLAG;
			else if(!check_if_command(data, temp))
				temp->type = COMMAND;
		}
		temp = temp->next;
	}
	print_type(data);
}