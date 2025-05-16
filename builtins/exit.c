/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:48:19 by jgamarra          #+#    #+#             */
/*   Updated: 2025/05/16 19:55:34 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_no_arguent(t_minishell *minishell)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	ft_putstr_fd(PROMPT_ERROR, STDERR_FILENO);
	ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
	minishell->status = 1;
}

void	handle_numeric_exit(t_cmd *cmd, t_minishell *minishell)
{
	t_execcmd	*ecmd;
	int			idx;

	ecmd = (t_execcmd *)cmd;
	idx = 0;
	while (ecmd->argv[1][idx])
	{
		if (!ft_isdigit(ecmd->argv[1][idx]))
		{
			ft_putstr_fd(PROMPT_ERROR, STDERR_FILENO);
			ft_putstr_fd("exit: numeric argument required\n", STDERR_FILENO);
			minishell->status = 255;
			return ;
		}
		idx++;
	}
	minishell->status = ft_atoi(ecmd->argv[1]) % 256;
}

/*
* Function to handle the exit command.
* It checks the number of arguments passed to the command.
* If there are no arguments, it exits with the last status.
*/
void	exit_impl(t_cmd *cmd, t_minishell *minishell)
{
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)cmd;
	if (!ecmd->argv[2])
	{
		if (ecmd->argv[1])
			handle_numeric_exit(cmd, minishell);
		exit(minishell->status);
	}
	else
		handle_no_arguent(minishell);
}
