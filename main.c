/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:34:17 by jgamarra          #+#    #+#             */
/*   Updated: 2025/06/07 21:12:38 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void check_leaks(void)
{
	system("leaks minishell");
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;
	char		*input;
	t_cmd		*cmd;
	char *tmp;

	prepare_minishell(&minishell);
	valid_inital_param(argc, envp, &minishell);
	// disable_echoctl();//  DISABLE FOR TESTER
	catch_signal();
	while (1)
	{
		minishell.error_syntax = false;
		// ONLY VALID FOR TESTER
		if (isatty(fileno(stdin)))
			input = readline(PROMPT);
		else
		{
			char	*line;
			line = get_next_line(fileno(stdin));
			if (!line)
				break ;
			input = ft_strtrim(line, "\n");
			free(line);
		}
		// ONLY VALID FOR TESTER

		catch_interactive(&minishell, input, "minishell$");
		input = check_input_valid(input);
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		if (!is_valid_quote_str(input, &minishell))
			continue ;
		// expand variables
		tmp = input;
		// save_history(tmp); // expand variables
		input = expand_variables_main(input, &minishell);
		// expand variables
		
		free(tmp); // expand variables
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		
		// printf("1_input: %s\n", input);
		cmd = parsecmd(input, &minishell);
		control_cmd(cmd, &minishell);
		free(input);
		// safe_free_minishell(&minishell);
		// atexit(check_leaks);
		// exit(0);
		// FREE EXECCMD FOR EACH WHILE LOOP
		// CHECK OTHER STRUCTURES
	}
	safe_free_minishell(&minishell);
}
