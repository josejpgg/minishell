/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_impl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:45:13 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/16 14:12:54 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void run_internal(t_cmd *cmd, t_minishell *minishell)
{
	t_execcmd *ecmd;
	t_echocmd *echo;
	int index;

	ecmd = (t_execcmd *)cmd;
	if (ft_strstr(ecmd->argv[0], "echo"))
	{
		echo = (t_echocmd *)cmd;
		if (echo->invalid)
		{
			minishell->status = 127;//IS OK????
			return ;
		}
		//valid position for echo
		if (echo->new_line)
		{
			index = 2;
		}
		else
		{
			index = 1;
		}
		while(ecmd->argv[index])
		{
			ft_putstr_fd(ecmd->argv[index++], 1);
		}
		if (!echo->new_line)
			ft_putchar_fd('\n', 1);
		minishell->status = 0;
	}
}

void run_external(t_cmd *cmd, t_minishell *minishell)
{
	int status;
	pid_t pid;

	if((pid = fork1()) == 0)
	{
		runcmd(cmd, minishell);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		minishell->status = WEXITSTATUS(status);
	}
}
