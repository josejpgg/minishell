/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_controller.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 21:50:39 by jgamarra          #+#    #+#             */
/*   Updated: 2025/04/27 22:09:52 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int valid_builtins(t_cmd *cmd)
{
	t_execcmd *ecmd;
	ecmd = (t_execcmd *)cmd;
	
	return (ft_strcmp(ecmd->argv[0], "echo") == 0 ||
			ft_strcmp(ecmd->argv[0], "cd") == 0 ||
			ft_strcmp(ecmd->argv[0], "pwd") == 0 ||
			ft_strcmp(ecmd->argv[0], "export") == 0 ||
			ft_strcmp(ecmd->argv[0], "unset") == 0 ||
			ft_strcmp(ecmd->argv[0], "env") == 0 ||
			ft_strcmp(ecmd->argv[0], "exit") == 0);
}

