/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_impl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:45:13 by jgamarra          #+#    #+#             */
/*   Updated: 2025/05/08 22:00:24 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void run_internal(t_cmd *cmd, t_minishell *minishell)
{
	t_execcmd *ecmd;
	int idx;
	int pos;
	
	ecmd = (t_execcmd *)cmd;
	minishell->status = 0;
	char quote=0;
	if (ft_strstr(ecmd->argv[0], "echo"))
	{
		echo_impl(cmd, minishell);
	}
	else if (ft_strstr(ecmd->argv[0], "pwd"))
	{
		pwd_impl(cmd, minishell);
	}
	else if (ft_strstr(ecmd->argv[0], "cd"))
	{
		cd_impl(cmd, minishell);
	}
	else if (ft_strstr(ecmd->argv[0], "env"))
	{
		env_impl(cmd, minishell);
	}
	else if (ft_strstr(ecmd->argv[0], "exit"))
	{
		exit_impl(cmd, minishell);
	}
	else if (ft_strstr(ecmd->argv[0], "export"))
	{
		export_impl(cmd, minishell);
	}
	else if (ft_strstr(ecmd->argv[0], "unset"))
	{
		unset_impl(cmd, minishell);
	}
	// free all variables from ecmd
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
