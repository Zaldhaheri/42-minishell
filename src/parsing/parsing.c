/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 20:13:00 by zaldhahe          #+#    #+#             */
/*   Updated: 2024/08/20 20:13:00 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	parser(t_data *data)
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
				add_token_from_checker(data, data->typeflag, &data->checker);
		}
		data->i++;
	}
	add_token_from_checker(data, data->typeflag, &data->checker);
	if (data->checker)
		free(data->checker);
}

void	append_checker(t_data *data)
{
	size_t	checker_len;
	size_t	new_size;

	checker_len = ft_strlen(data->checker);
	new_size = checker_len + 2;
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

void	add_token_from_checker(t_data *data, int type, char **str)
{
	t_token	*curr;

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

void	append_checker_char(t_data *data, int c)
{
	size_t	checker_len;
	size_t	new_size;

	checker_len = ft_strlen(data->checker);
	new_size = checker_len + 2;
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

int	check_string(t_data *data)
{
	if (data->input[data->i] == '<')
		return (parse_in(data));
	else if (data->input[data->i] == '>')
		return (parse_out(data));
	else if (data->input[data->i] == '|')
		return (parse_pipe(data));
	else if (data->input[data->i] == '$')
		return (parse_dollar(data));
	else if (data->input[data->i] == ' ' || data->input[data->i] == '\t'
		|| ft_strlen(data->input) - 1 <= data->i)
		return (parse_space(data));
	return (0);
}
