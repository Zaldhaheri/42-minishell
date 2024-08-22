/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalkhate <nalkhate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 21:09:48 by zaldhahe          #+#    #+#             */
/*   Updated: 2024/08/20 21:43:56 by nalkhate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_H
# define DEFINE_H

# define WORD 0
# define COMMAND 11 //execve
# define BOTH_FD 9
# define FD_IN 12
# define FD_OUT 13
# define APPEND 14
# define HEREDOC 15
# define FILENAME 16
# define LIMITER 17
# define PIPE 18
# define DQUOTES 19
# define SQUOTES 20
# define DOLLAR 21
# define DECLARE 22
# define BCOMMAND 23 //builtin
# define FLAG 24
# define NO_FD -10

# define RESET   "\033[0m"
# define BLACK   "\033[30m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN    "\033[36m"
# define WHITE   "\033[37m"

#endif