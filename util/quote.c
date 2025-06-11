/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 17:29:17 by jgamarra          #+#    #+#             */
/*   Updated: 2025/06/07 20:57:54 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char check_argument(t_execcmd *ecmd, int idx)
{
	int pos;
	char quote;	
	
	pos = 0;
	while (ecmd->argv[idx][pos])
	{
		if (!quote && (ecmd->argv[idx][pos] == '\'' || ecmd->argv[idx][pos] == '\"'))
			quote = ecmd->argv[idx][pos];
		else if (quote && ecmd->argv[idx][pos] == quote)
			quote = 0;
		pos++;
	}
	return (quote);
}

char check_argument_str(char *cmd)
{
	int pos;
	char quote;	
	
	pos = 0;
	quote = 0;
	while (cmd[pos])
	{
		if (!quote && (cmd[pos] == '\'' || cmd[pos] == '\"'))
			quote = cmd[pos];
		else if (quote && cmd[pos] == quote)
			quote = 0;
		pos++;
	}
	return (quote);
}

int is_valid_quote(t_cmd *cmd, t_minishell *minishell)
{
	char quote;
	int idx;
	t_execcmd *ecmd;
	
	ecmd = (t_execcmd *)cmd;
	idx = 1;
	quote = 0;
	while (ecmd->argv[idx])
	{
		quote = check_argument(ecmd, idx);
		idx++;
	}
	if (quote)
	{
		ft_putstr_fd("Error: Unmatched quote\n", STDERR_FILENO);
		minishell->status = 1;
		return (0);
	}
	return (1);
}

int is_valid_quote_str(char *cmd, t_minishell *minishell)
{
	char quote;
	
	quote = check_argument_str(cmd);
	if (quote)
	{
		ft_putstr_fd("Error: Unmatched quote\n", STDERR_FILENO);
		minishell->status = 1;
		return (0);
	}
	return (1);
}
