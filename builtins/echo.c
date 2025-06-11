/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:48:19 by jgamarra          #+#    #+#             */
/*   Updated: 2025/06/07 21:09:17 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo_impl(t_cmd *cmd, t_minishell *minishell)
{
	int			idx;
	t_execcmd	*ecmd;
	int			new_line;

	ecmd = (t_execcmd *)cmd;
	// print_vector(ecmd->argv);
	new_line = 1;
	idx = 0;
	while (ecmd->argv[++idx] && ft_strstr(ecmd->argv[idx], "-n"))
		new_line = 0;
	char *tmp; 
	// print_vector(ecmd->argv);
	while (ecmd->argv[idx])
	{
		// print_vector(ecmd->argv);
		if (!is_valid_quote(cmd, minishell))
			return ;
		tmp = expand_variables(ecmd->argv[idx], minishell);
		// remove_quotes(ecmd, idx); PARECE QUE SE PUEDE QUITAR
		// tmp = ecmd->argv[idx];
		ft_putstr_fd(tmp, STDOUT_FILENO);
		if (ecmd->argv[idx + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		// free(ecmd->argv[idx]);  PARECE QUE SE PUEDE QUITAR
		idx++;
		// free(tmp);
	}
	if (new_line)
		ft_putstr_fd("\n", STDOUT_FILENO);
	minishell->status = 0;
}
