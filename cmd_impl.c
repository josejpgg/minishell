/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_impl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:45:13 by jgamarra          #+#    #+#             */
/*   Updated: 2025/05/04 20:32:16 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void run_internal(t_cmd *cmd, t_minishell *minishell)
{
	t_execcmd *ecmd;
	int idx;
	int pos;
	
	ecmd = (t_execcmd *)cmd;
	minishell->status = 0;
	// print_vector(ecmd->argv);
	char quote=0;
	if (ft_strstr(ecmd->argv[0], "echo"))
	{
		// minishell->status = 0;
		// quote validator
		if (!is_valid_quote(cmd, minishell))
		{
			return ;
		}
		// char quote=0;
		// idx = 1;
		// while (ecmd->argv[idx])
		// {
		// 	pos = 0;
		// 	while (ecmd->argv[idx][pos])
		// 	{
		// 		if (!quote && (ecmd->argv[idx][pos] == '\'' || ecmd->argv[idx][pos] == '\"'))
		// 			quote = ecmd->argv[idx][pos];
		// 		else if (quote && ecmd->argv[idx][pos] == quote)
		// 			quote = 0;
		// 		pos++;
		// 	}
		// 	idx++;
		// }
		// if (quote)
		// {
		// 	write(2, "Error: Unmatched quote\n", 23);
		// }
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

				// control $
				if (quote != '\'' && ecmd->argv[idx][pos] == '$')
				{
					pos++;
					// control $?
					if (ecmd->argv[idx][pos] == '?')
					{
						write(1, ft_itoa(minishell->status), ft_strlen(ft_itoa(minishell->status)));
						pos++;
						continue ;	
					}
					// check if variable is valid. [a-zA-Z_] and not \ or "
					else if ((ft_isalnum(ecmd->argv[idx][pos]) || ecmd->argv[idx][pos] == '_') && ecmd->argv[idx][pos] != '\\' && ecmd->argv[idx][pos] != '"')
					{
						// printf("here\n");
						// EXPAND ENVIRONMENT VARIABLES
						// int variable = 0;
						// while (ecmd->argv[idx][pos + variable] && (ft_isalnum(ecmd->argv[idx][pos + variable]) || ecmd->argv[idx][pos + variable] == '_'))
						// 	variable++;
						// char *var = ft_substr(ecmd->argv[idx], pos, variable);
						// char *var_value = getenv(var);
						// if (var_value)
						// 	write(1, var_value, ft_strlen(var_value));
						// else
						// 	write(1, "", 1);// if variable not found print empty string
						// pos += variable;
						print_expand_variable(cmd, idx, &pos, minishell);
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
		minishell->status = 0;
	}
	else if (ft_strstr(ecmd->argv[0], "pwd"))
	{
		pwd_impl(cmd, minishell);
	}
	else if (ft_strstr(ecmd->argv[0], "cd"))
	{
		cd_impl(cmd, minishell);
	}
	else if (ft_strstr(ecmd->argv[0], "env"))
	{
		env_impl(cmd, minishell);
	}
	else if (ft_strstr(ecmd->argv[0], "exit"))
	{
		exit_impl(cmd, minishell);
	}
	else if (ft_strstr(ecmd->argv[0], "export"))
	{
		export_impl(cmd, minishell);
		
		return ;

		idx = 1;
		
		if (!ecmd->argv[idx])
		{
			// print all environment variables sorted
			printf("declare -x \n");
		}
		while (ecmd->argv[idx])
		{
			// valid if quote is valid
			if (!is_valid_quote(cmd, minishell))
				return ;
			ecmd->argv[idx] = expand_variables(ecmd->argv[idx], minishell);
			// remove quotes if exists
			remove_quotes(ecmd, idx);
			// check if value is valid
			int s = -1;
			char **q;
			char *arge[3];
			int argc = 0;
			q = &ecmd->argv[idx];
			int exported = 0;
			printf("here\n");
			while (++s < ft_strlen(ecmd->argv[idx]) && ecmd->argv[idx][s])
			{
				printf("s = %d\n", s);
				printf("ecmd->argv[idx][++s] = %c\n", ecmd->argv[idx][s]);
				if (s == 0 && ft_isalpha(ecmd->argv[idx][s]) || ecmd->argv[idx][s] == '_')
				{
					if (ecmd->argv[idx][s + 1])
						continue ;
				}
				else if (s != 0 && ft_isalnum(ecmd->argv[idx][s]) || ecmd->argv[idx][s] == '_')
				{
					printf("here3\n");
					if (ecmd->argv[idx][s + 1])
						continue ;
					else
						arge[argc++] = *q;
				}
				else
				{
					printf("here2\n");
					if (s != 0 && ecmd->argv[idx][s] == '=')
					{
						ecmd->argv[idx][s] = 0;
						arge[argc++] = *q;
						*q = *q + s + 1;
						exported = 1;
						while (ecmd->argv[idx][++s])
							;					
						if (!ecmd->argv[idx][s + 1])
							arge[argc++] = *q;
						break ;
					}
					// print error if variable is not valid
					printf("export: `%s': not a valid identifier\n", ecmd->argv[idx]);
					minishell->status = 1;
					return ;
				}
				break ;
				printf("here1\n");
			}
			printf("here4\n");
			arge[argc] = 0;
			printf("here5\n");

			// check if variable already exists
			if (env_exists(minishell, arge[0]))
			{
			printf("here6\n");

				//update variable if already exists
				update_env_value(minishell, arge[0], arge[1], exported);
			}
			else
			{
			printf("here7\n");	
				//add variable to environment
				create_env_value(minishell, arge[0], arge[1], exported);
			}
			idx++;	
		}
		minishell->status = 0;
	}
	else if (ft_strstr(ecmd->argv[0], "unset"))
	{
		// TODO Valid no options
		idx = 1;
		while (ecmd->argv[idx])
		{
			// valid if quote is valid
			if (!is_valid_quote(cmd, minishell))
				return ;
			ecmd->argv[idx] = expand_variables(ecmd->argv[idx], minishell);
			// remove quotes if exists
			remove_quotes(ecmd, idx);
			
			
			// check if value is valid
			int s = -1;
			while (ecmd->argv[idx][++s])
			{
				if (s == 0 && ft_isalpha(ecmd->argv[idx][s]) || ecmd->argv[idx][s] == '_')
				{
					if (ecmd->argv[idx][s + 1])
						continue ;
				}
				else if (s != 0 && ft_isalnum(ecmd->argv[idx][s]) || ecmd->argv[idx][s] == '_')
				{
					if (ecmd->argv[idx][s + 1])
						continue ;
				}
				else
				{
					// print error if variable is not valid
					printf("unset: `%s': not a valid identifier\n", ecmd->argv[idx]);
					minishell->status = 1;
					return ;
				}
				
			}
			// remove variable from environment
			remove_env_value(minishell, ecmd->argv[idx]);
			idx++;
		}
		minishell->status = 0;
	}
	// free all variables from ecmd
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
