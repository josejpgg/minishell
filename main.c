/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:34:17 by jgamarra          #+#    #+#             */
/*   Updated: 2025/05/08 22:48:41 by jgamarra         ###   ########.fr       */
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
	struct cmd *cmd;

	prepare_minishell(&minishell);
	valid_inital_param(argc, envp, &minishell);

	// history
	minishell.history = history_create();
    load_history_file(minishell.history, ".minishell_history");
	// history

	//  DISABLE FOR TESTER
	disable_echoctl();
	//  DISABLE FOR TESTER
	catch_signal();
	while (1)
	{

		// ONLY VALID FOR TESTER
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
		// ONLY VALID FOR TESTER

		// catch_interactive(input, PROMPT); DO NOT REMOVE
		catch_interactive(&minishell, input, "minishell$"); // ctrl + d

		input = check_input_valid(input);
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}


		// history
		// Si la entrada no es vacía, la agregamos al historial siempre y cuando
        // no se trate de un comando de historial, para evitar contaminarlo.
		if (strncmp(input, "history", 7) != 0) {
			// add_history(input);
			history_add(minishell.history, input);
		}
		// history

		// history
		if (strncmp(input, "history", 7) == 0) {
			// Se extrae el argumento (si existe) después de "history"
			char *arg = input + 7;
			// Saltamos cualquier espacio en blanco
			while (*arg && ft_isspace((unsigned char)*arg))
				arg++;

			// Si el argumento no es vacío se lo pasa a history_print,
			// de lo contrario se imprime el historial completo.
			history_print(minishell.history, ((*arg) != '\0') ? arg : NULL);
			free(input);
			continue;
		}
		// history


		// catch_interactive(&minishell, input, "minishell$"); // ctrl + d not works
		// char *test = malloc(1000);
		// pgm opt1 opt2 < file1 > file2
		// atexit(check_leaks);
		// exit(0);
		cmd = parsecmd(input);
		control_cmd(cmd, &minishell);
		free(input);
		// safe_free_minishell(&minishell);
		// atexit(check_leaks);
		// exit(0);
	}
	safe_free_minishell(&minishell);
	// atexit(check_leaks);
	// exit(0);
}
