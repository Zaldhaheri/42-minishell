#include "../../inc/minishell.h"

void parser(t_data *data)
{
	while (data->input[data->i])
	{
		if (data->input[data->i] == '\"')
			parse_double_quotes(data);
		else if (data->input[data->i] == '\'')
			parse_single_quotes(data);
		else
		{
			append_checker(data);
			if (check_string(data))
			{
				// if (ft_strrchr(data->checker, '=') && ft_strlen(data->checker) > 1
				//     && data->checker[0] != '=' && data->typeflag != DOLLAR && is_valid_key(data->checker))
				//     add_token_to_env(data);
				// else
				add_token_from_checker(data, data->typeflag, &data->checker);
			}
		}
		data->i++;
	}
	add_token_from_checker(data, data->typeflag, &data->checker);
	//ft_lstclear(data);
	if (data->checker)
		free(data->checker);
}

void append_checker(t_data *data)
{
	size_t checker_len = ft_strlen(data->checker);
	size_t new_size = checker_len + 2;
	data->temp = malloc(new_size);
	if (!data->temp)
	{
		perror("malloc");
		exit(1);
	}
	ft_strcpy(data->temp, data->checker);
	data->temp[checker_len] = data->input[data->i];
	data->temp[checker_len + 1] = 0;
	if (data->checker)
		free(data->checker);
	data->checker = data->temp;
}

void add_token_from_checker(t_data *data, int type, char **str)
{
	t_token *curr;

	if (ft_strlen(*str) > 0)
	{
		curr = ft_lstnew(*str);
		curr->type = type;
		ft_lstadd_back(&data->tokens, curr);
		if (*str)
			free(*str);
		*str = ft_strdup("");
		data->typeflag = WORD;
	}
}

void append_checker_char(t_data *data, int c)
{
	size_t checker_len = ft_strlen(data->checker);
	size_t new_size = checker_len + 2; // +1 for new char, +1 for null terminator
	data->temp = malloc(new_size);
	if (!data->temp)
	{
		perror("malloc");
		exit(1);
	}
	ft_strcpy(data->temp, data->checker);
	data->temp[checker_len] = c;
	data->temp[checker_len + 1] = 0;
	if (data->checker)
		if (data->checker)
			free(data->checker);
	data->checker = data->temp;
}

int parse_dollar(t_data *data)
{
	char *key;
	char *value;
	char *temp;
	int j;

	j = 0;
	key = malloc(256);
	ft_memset(key, 0, 256);
	data->i++;
	data->checker[ft_strlen(data->checker) - 1] = '\0';
	while(data->input[data->i] && (ft_isalnum(data->input[data->i]) || data->input[data->i] == '_' || data->input[data->i] == '?'))
		key[j++] = data->input[data->i++];
	key[j] = 0;
	value = get_env_value(data, key);
	if (value)
	{
		temp = value;
		while (*temp)
		{
			append_checker_char(data, *temp);
			temp++;
		}
	}
	data->i--;
	data->typeflag = DOLLAR;
	if (key)
		free(key);
	if (data->input[data->i + 1] == ' ')
		return (1);
	return (0);
}

int check_string(t_data *data)
{
	if (data->input[data->i] == '<')
		return (parse_in(data));
	else if (data->input[data->i] == '>')
		return (parse_out(data));
	else if (data->input[data->i] == '|')
		return (parse_pipe(data));
	else if (data->input[data->i] == '$')
		return (parse_dollar(data));
	else if (data->input[data->i] == ' ' || data->input[data->i] == '\t' || 
			ft_strlen(data->input) - 1 <= data->i)
		return (parse_space(data));
	return (0);
}
