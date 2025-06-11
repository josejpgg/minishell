/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_controller.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:27:25 by jgamarra          #+#    #+#             */
/*   Updated: 2025/06/07 13:38:15 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	search_in_path(t_execcmd *ecmd, t_minishell *minishell)
{
	int		i;
	char	*cmd;
	char	*full;

	i = 0;
	while (minishell->path_env[i])
	{
		cmd = ft_strjoin("/", ecmd->argv[0]);
		full = ft_strjoin(minishell->path_env[i], cmd);
		free(cmd);
		if (access(full, F_OK) == 0)
		{
			ecmd->argv[0] = full;
			return ;
		}
		free(full);
		i++;
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(ecmd->argv[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}

void	valid_command(t_execcmd *ecmd, t_minishell *minishell)
{
	if (ft_strchr("./", ecmd->argv[0][0]))
	{
		exit(handle_access_error(ecmd->argv[0]));
	}
	search_in_path(ecmd, minishell);
}

void	exec_command(char *command, char **args)
{
	if (execvp(command, args) == -1)
	{
		exit(handle_error(command));
	}
}

void	control_cmd(t_cmd *cmd, t_minishell *minishell)
{
	if (minishell->error_syntax)
		return ;
	if (valid_builtins(cmd))
		run_internal(cmd, minishell);
	else
		run_external(cmd, minishell);
}
