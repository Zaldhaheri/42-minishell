
#include "../exec/executor.h"
//bcommands execute here
void bcomm_exec(t_command *cmd, t_data *data)
{
	ft_putstr_fd("bcomm_exec\n", 2);
	if (!ft_strncmp("echo", cmd->command[0], 4))
		b_echo(cmd->command);
	else if (!ft_strncmp("env", cmd->command[0], 3))
		b_env(data);
	else if (!ft_strncmp("export", cmd->command[0], 6))
		b_export(data, cmd->command);
	else if (!ft_strncmp("unset", cmd->command[0], 5))
		b_unset(data, cmd->command);
    else if (!ft_strncmp("cd", cmd->command[0], 2))
		b_cd(data, cmd->command);
    else if (!ft_strncmp("pwd", cmd->command[0], 3))
		b_pwd();
    else if (!ft_strncmp("exit", cmd->command[0], 4))
		b_exit(data, cmd);
	else if (ft_strrchr(cmd->command[0], '=') && ft_strlen(cmd->command[0]) > 1
                    && cmd->command[0][0] != '=' && is_valid_key(cmd->command[0]))
			b_declare(data, cmd->command);
}