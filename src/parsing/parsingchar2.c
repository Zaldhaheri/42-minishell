/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingchar2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaldhahe <zaldhahe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 20:01:00 by zaldhahe          #+#    #+#             */
/*   Updated: 2024/08/20 20:20:43 by zaldhahe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	parse_pipe(t_data *data)
{
	data->checker[ft_strlen(data->checker) - 1] = '\0';
	if (data->i > 0 && ft_strlen(data->checker) != 0
		&& data->input[data->i - 1] != ' ' && data->input[data->i - 1] != '\t')
		add_token_from_checker(data, WORD, &data->checker);
	ft_lstadd_back(&data->tokens, ft_lstnew("|"));
	return (1);
}

int	check_key(t_data *data)
{
	return (data->input[data->i] && (ft_isalnum(data->input[data->i])
			|| data->input[data->i] == '_' || data->input[data->i] == '?'));
}

int	parse_dollar(t_data *data)
{
	data->j = 0;
	data->tempkey = malloc(256);
	ft_memset(data->tempkey, 0, 256);
	data->i++;
	data->checker[ft_strlen(data->checker) - 1] = '\0';
	while (check_key(data))
		data->tempkey[data->j++] = data->input[data->i++];
	data->tempkey[data->j] = 0;
	data->tempvalue = get_env_value(data, data->tempkey);
	if (data->tempvalue)
	{
		data->temp2 = data->tempvalue;
		while (*(data->temp2))
		{
			append_checker_char(data, *(data->temp2));
			data->temp2++;
		}
	}
	data->i--;
	data->typeflag = DOLLAR;
	if (data->tempkey)
		free(data->tempkey);
	if (data->input[data->i + 1] == ' ')
		return (1);
	return (0);
}
