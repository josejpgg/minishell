/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_controller.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:39:59 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/15 21:21:42 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int valid_builtins(t_cmd *cmd)
{
	t_execcmd *ecmd;
	ecmd = (t_execcmd *)cmd;
	
	return (ft_strstr(ecmd->argv[0], "echo")||
			ft_strstr(ecmd->argv[0], "cd")||
			ft_strstr(ecmd->argv[0], "pwd")||
			ft_strstr(ecmd->argv[0], "export")||
			ft_strstr(ecmd->argv[0], "unset")||
			ft_strstr(ecmd->argv[0], "env")||
			ft_strstr(ecmd->argv[0], "exit"));
}

t_cmd *prepare_builtins(t_cmd *cmd, t_minishell *minishell)
{
	int internal;
	t_execcmd *ecmd;
	t_echocmd *echo;

	internal = 1;
	ecmd = (t_execcmd *)cmd;
	if (ft_strstr(ecmd->argv[0], "echo"))
	{
		echo = (t_echocmd *)cmd;
		echo->type = ECHO;
		echo->new_line = false;
		echo->simple_quote = false;
		echo->double_quote = false;
		if (ft_strncmp(ecmd->argv[1], "-n", 2) == 0)
		{
			echo->new_line = true;
		}
		int vectorlen = ft_vector_size(ecmd->argv);
		int vectorindex = 0;
		// remove extra -n TODO
		// remove quotes TODO
		if (vectorlen > 1)
		{
			// valid quotes
			while (ecmd->argv[++vectorindex])
			{
				if (ft_strchr(ecmd->argv[vectorindex], '\''))
				{
					echo->simple_quote = true;
				}
				if (ft_strchr(ecmd->argv[vectorindex], '\"'))
				{
					echo->double_quote = true;
				}
				// valid symbol $?
				if (echo->double_quote)
				{
					if (ft_strstr(ecmd->argv[vectorindex], "$?"))
					{
						// replace the character $? with a value
						ecmd->argv[vectorindex] = ft_strreplace(ecmd->argv[vectorindex], "$?", ft_itoa(minishell->status));
					}
				}
			}
		}
		print_vector(ecmd->argv);
		return((t_cmd *)echo);
		// ft_echo(ecmd->argv);
		// print_vector(minishell->env);
		// ft_printf("%d\n", ecmd->minishell->status);
		// exit(0);
	}
	else if (ft_strstr(ecmd->argv[0], "cd"))
	{
		// ft_cd(ecmd->argv, ecmd->minishell);
		exit(0);
	}
	else if (ft_strstr(ecmd->argv[0], "pwd"))
	{
		// ft_pwd();
		exit(0);
	}
	else if (ft_strstr(ecmd->argv[0], "export"))
	{
		// ft_export(ecmd->argv, ecmd->minishell);
		exit(0);
	}
	else if (ft_strstr(ecmd->argv[0], "unset"))
	{
		// ft_unset(ecmd->argv, ecmd->minishell);
		exit(0);
	}
	else if (ft_strstr(ecmd->argv[0], "env"))
	{
		// ft_env(ecmd->minishell->env);
		exit(0);
	}
	// else if (ft_strstr(ecmd->argv[0], "exit"))
	// {
	// 	// ft_exit(ecmd->minishell->env);
	// 	exit(0);
	// }
	return (cmd);
}

// Function to replace a string with another string
// the first value is the main string
// the second value is the string to be find and replaced
// the third value is the string to replace
char *ft_strreplace(char *str, char *old, char *new)
{
	char *result;
	int i, count = 0;
	int newlen = ft_strlen(new);
	int oldlen = ft_strlen(old);

	// Counting the number of times old word
	// occur in the string
	for (i = 0; str[i] != '\0'; i++)
	{
		if (ft_strstr(&str[i], old) == &str[i])
		{
			count++;
			// Jumping to index after the old word.
			i += oldlen - 1;
		}
	}

	// Making new string of enough length
	result = (char *)safe_malloc(i + count * (newlen - oldlen) + 1);

	i = 0;
	while (*str)
	{
		// compare the substring with the result
		if (ft_strstr(str, old) == str)
		{
			strcpy(&result[i], new);
			i += newlen;
			str += oldlen;
		}
		else
			result[i++] = *str++;
	}
	result[i] = '\0';
	return (result);
}
