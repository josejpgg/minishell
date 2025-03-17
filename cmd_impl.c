/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_impl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:45:13 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/17 22:06:30 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void run_internal(t_cmd *cmd, t_minishell *minishell)
{
	t_execcmd *ecmd;
	int idx;
	int pos;
	
	ecmd = (t_execcmd *)cmd;
	// print_vector(ecmd->argv);
	if (ft_strstr(ecmd->argv[0], "echo"))
	{
		// quote validator
		char quote=0;
		idx = 1;
		while (ecmd->argv[idx])
		{
			pos = 0;
			while (ecmd->argv[idx][pos])
			{
				if (!quote && (ecmd->argv[idx][pos] == '\'' || ecmd->argv[idx][pos] == '\"'))
					quote = ecmd->argv[idx][pos];
				else if (quote && ecmd->argv[idx][pos] == quote)
					quote = 0;
				pos++;
			}
			idx++;
		}
		if (quote)
		{
			write(2, "Error: Unmatched quote\n", 23);
		}
		// quote validator
		
		// new line validator
		int new_line = 1;
		idx = 1;
		while (ecmd->argv[idx] && ft_strstr(ecmd->argv[idx], "-n"))
		{
			new_line = 0;
			idx++;
		}
		// new line validator

		// print echo
		if (!ecmd->argv[idx])
			write(1, "", 1);
		while (ecmd->argv[idx])
		{
			pos = 0;
			// printf("control: (%s)\n", ecmd->argv[idx]);
			while (ecmd->argv[idx][pos])
			{
				// control quote
				if (!quote && (ecmd->argv[idx][pos] == '\'' || ecmd->argv[idx][pos] == '\"'))
				{
					quote = ecmd->argv[idx][pos];
					pos++;
					continue ;
				}
				else if (quote && ecmd->argv[idx][pos] == quote)
				{
					quote = 0;
					pos++;
					continue ;
				}
				// control quote
				if (quote != '\'' && ecmd->argv[idx][pos] == '$')
				{
					pos++;
					if (ecmd->argv[idx][pos] == '?')
					{
						write(1, ft_itoa(minishell->status), ft_strlen(ft_itoa(minishell->status)));
						pos++;
						continue ;	
					}
					else if ((ft_isalnum(ecmd->argv[idx][pos]) || ecmd->argv[idx][pos] == '_') && ecmd->argv[idx][pos] != '\\' && ecmd->argv[idx][pos] != '"')
					{
						// printf("here\n");
						// EXPAND ENVIRONMENT VARIABLES
						int variable = 0;
						while (ecmd->argv[idx][pos + variable] && (ft_isalnum(ecmd->argv[idx][pos + variable]) || ecmd->argv[idx][pos + variable] == '_'))
							variable++;
						char *var = ft_substr(ecmd->argv[idx], pos, variable);
						char *var_value = getenv(var);
						if (var_value)
							write(1, var_value, ft_strlen(var_value));
						else
							write(1, "", 1);
						pos += variable;
						continue ;
						// EXPAND ENVIRONMENT VARIABLES
					}
					else
					{
						// printf("here2: (%c)\n",ecmd->argv[idx][pos - 1]);
						// print previous value $
						write (1, &ecmd->argv[idx][pos - 1], 1);
						if (ecmd->argv[idx][pos] == '\\' || ecmd->argv[idx][pos] == '"')
						{
							continue ;
						}
						else
						{
							write (1, &ecmd->argv[idx][pos], 1);
							pos++;
							continue ;
						}
						// // not print quotes
						// if (!quote)
						// 	write (1, &ecmd->argv[idx][pos], 1);
						// // move to next value not current " $ "
						// if (quote)
						// 	pos++;
					}
				}
				// ignore "\"
				/*
				No interpretar comillas sin cerrar o caracteres especiales no especificados en el enunciado como \ (barra invertida) o ; (punto y coma).
				*/
				if (ecmd->argv[idx][pos] == '\\')
				{
					pos++;
					continue ;
				}
				// printf("here3: (%c)\n",ecmd->argv[idx][pos]);
				write(1, &ecmd->argv[idx][pos], 1);
				pos++;
			}
			// print if next element is not empty
			if (ecmd->argv[idx + 1])
				write(1, " ", 1);
			idx++;
		}
		if (new_line)
			write(1, "\n", 1);
		// print echo
	}
	else if (ft_strstr(ecmd->argv[0], "pwd"))
	{
		char *pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			printf("pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
			minishell->status = 1;
			return ;
		}
		write(1, pwd, ft_strlen(pwd));
		write(1, "\n", 1);
		free(pwd);
		minishell->status = 0;
	}
	else if (ft_strstr(ecmd->argv[0], "cd"))
	{
		// no more than 1 argument
		minishell->status = 0;
		if (ft_vector_size(ecmd->argv) > 2)
		{
			printf("cd: too many arguments\n");
			minishell->status = 1;
			return ;
		}
		else if (ft_vector_size(ecmd->argv) < 2)
		{
			// not only one param. IMPLEMENT GO HOME????
			printf("cd: syntax error\n");
			minishell->status = 1;
			return ;
		}
		// move directory
		if (chdir(ecmd->argv[1]) != 0)
		{
			printf("cd: no such file or directory: %s\n", ecmd->argv[1]);
			minishell->status = 1;
		}
		set_env_value(minishell, "OLDPWD", get_env_value(minishell, "PWD"));
		set_env_value(minishell, "PWD", getcwd(NULL, 0));
	}
	else if (ft_strstr(ecmd->argv[0], "env"))
	{
		int i = 0;
		while (minishell->env[i])
		{
			write(1, minishell->env[i], ft_strlen(minishell->env[i]));
			write(1, "\n", 1);
			i++;
		}
	}
	else if (ft_strstr(ecmd->argv[0], "exit"))
	{
		exit(0);
	}
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
