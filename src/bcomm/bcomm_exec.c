
#include "../exec/executor.h"
//bcommands execute here
void bcomm_exec(t_command *cmd, t_data *data)
{
	if (!ft_strncmp("echo", cmd->command[0], 4))
		b_echo(cmd->command);
	else if (!ft_strncmp("env", cmd->command[0], 3))
		b_env(data);
    else if (!ft_strncmp("cd", cmd->command[0], 2))
		b_cd(data, cmd->command);
    else if (!ft_strncmp("pwd", cmd->command[0], 3))
		b_pwd();
    
}