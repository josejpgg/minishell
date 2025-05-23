/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 17:04:21 by jgamarra          #+#    #+#             */
/*   Updated: 2025/05/23 19:13:57 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safe_free_vector(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
	{
		free(split[i]);
	}
	if (split)
		free(split);
}

void	safe_free_minishell(t_minishell *minishell)
{
	if (minishell->path_env)
		safe_free_vector(minishell->path_env);
	if (minishell->env)
		safe_free_vector(minishell->env);
	free(minishell->exported);
}

void free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return;

	if (cmd->type == EXEC)
	{
		printf("here1\n");
		t_execcmd *ecmd = (t_execcmd *)cmd;
		if (ecmd->argv)
		{
			printf("here2\n");
			// free(ecmd->argv[0]);
			// for (int i = 0; ecmd->argv[i]; i++)
			// 	free(ecmd->argv[i]);
			printf("here3\n");
			free(ecmd->argv);
		}
	}
	else if (cmd->type == PIPE)
	{
		t_pipecmd *pcmd = (t_pipecmd *)cmd;
		free_cmd(pcmd->left);
		free_cmd(pcmd->right);
	}
	else if (cmd->type == REDIR)
	{
		t_redircmd *rcmd = (t_redircmd *)cmd;
		free_cmd(rcmd->cmd);
		free(rcmd->file);
	}
	printf("here4\n");
	// free(cmd);
}

