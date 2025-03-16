/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:34:17 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/16 14:07:02 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;
	char		*input;
	struct cmd *cmd;

	prepare_minishell(&minishell);
	valid_inital_param(argc, envp, &minishell);
	catch_signal();
	while (1)
	{
		// REPLACE COMMENT FOR TESTER
		if (isatty(fileno(stdin)))
			input = readline(PROMPT);
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			if (!line)
				break ;
			input = ft_strtrim(line, "\n");
			free(line);
		}
		// REPLACE COMMENT FOR TESTER
		catch_interactive(input, PROMPT);
		input = check_input_valid(input);
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
    // pgm opt1 opt2 < file1 > file2
    cmd = parsecmd(input);
	control_cmd(cmd, &minishell);
    free(input);
	}
	safe_free_minishell(&minishell);
}
 