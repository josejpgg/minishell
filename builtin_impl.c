/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_impl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 16:49:52 by jgamarra          #+#    #+#             */
/*   Updated: 2025/05/23 19:32:03 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_variables(char *arg, t_minishell *minishell)
{
	char *s = arg, *result = ft_strdup(""), *var, tmp[2] = {'\0', '\0'};
	char quote = 0, *tmp_result;

	while (*s)
	{
		if (!quote && (*s == '\'' || *s == '\"'))
		{
			quote = *s++;
			continue;
		}
		else if (quote && *s == quote)
		{
			quote = 0, s++;
			continue;
		}
		else if (*s == '$' && quote != '\'')
		{
			s++;
			if (*s == '?')
				var = ft_itoa(minishell->status), s++;
			else if (*s == '_' || ft_isalpha(*s))
			{
				int len = 0; while (s[len] && (ft_isalnum(s[len]) || s[len] == '_')) len++;
				var = ft_substr(s, 0, len);
				char *val = getenv_minishell(minishell, var);
				free(var); var = ft_strdup(val);
				s += len;
			}
			else continue;
		}
		else
			tmp[0] = *s++, var = ft_strdup(tmp);
		
		tmp_result = ft_strjoin(result, var);
		free(result); free(var);
		result = tmp_result;
	}
	return result;
}


char *expand_variables2(char *arg, t_minishell *minishell)
{
	char *s;
	char *es;
	int idx = 1;
	char quote;
	char *result;
	char *var_name;
	char tmp[2];
	char *tmp_result;

	
	tmp[1] = '\0';
	
	result = ft_strdup("");
	s = arg;
	es = s + ft_strlen(arg);
	quote = 0;
	while (*s)
	{
		// printf("result = %s\n", result);
		// control quote. do not handle quote or backslash
		if (!quote && (*s == '\'' || *s == '\"'))
		{
			tmp[0] = *s;
			tmp_result = ft_strjoin(result, tmp);
			free(result);
			result = ft_strdup(tmp_result);
			free(tmp_result);
			quote = *s;
			s++;
			continue ;
		}
		else if (quote && *s == quote)
		{
			tmp[0] = *s;
			tmp_result = ft_strjoin(result, tmp);
			free(result);
			result = ft_strdup(tmp_result);
			free(tmp_result);
			quote = 0;
			s++;
			continue ;
		}
		// control quote
		// control $ if start like a valid variable
		if (quote != '\'' && *s == '$')
		{
			s++;
			if (*s == '?')
			{
				tmp_result = ft_strjoin(result, ft_itoa(minishell->status));
				free(result);
				result = ft_strdup(tmp_result);
				free(tmp_result);
				s++;
				continue ;
			}
			else if (*(s) == '_' || ft_isalpha(*(s)))
			{
				var_name = ft_strdup("");
				while (*s && (*s == '_' || ft_isalnum(*s)))
				{
					tmp[0] = *s;
					var_name = ft_strjoin(var_name, tmp);
					s++;
				}
				tmp_result = ft_strjoin(result, getenv_minishell(minishell, var_name));
				free(result);
				result = ft_strdup(tmp_result);
				free(tmp_result);
				free(var_name);
				continue ;
			}
			s++;
		}
		tmp[0] = *s;
		tmp_result = ft_strjoin(result, tmp);
		free(result);
		result = ft_strdup(tmp_result);
		free(tmp_result);
		s++;
	}
	if (s == es)
	{
		arg = result;
	}
	atexit(check_leaks);
	exit(0);
	return arg;
}

void split_variables(t_cmd *cmd, t_minishell *minishell, char c)
{
	t_execcmd *ecmd;
	char **split;

	ecmd = (t_execcmd *)cmd;
	
	
}

void remove_quotes(t_execcmd *ecmd, int idx)
{
	char *result;
	char tmp[2];
	char *tmp_result;
	char *str;

	result= ft_strdup("");
	tmp[1] = '\0';
	str = ecmd->argv[idx];
	while (*str)
	{
		if (*str != '\'' && *str != '\"')
		{
			tmp[0] = *str;
			tmp_result = ft_strjoin(result, tmp);
			free(result);
			result = tmp_result;
		}
		str++;
	}
	free(ecmd->argv[idx]);
	ecmd->argv[idx] = result;
}

char *remove_quotes_simple(char *str)
{
	char *result;
	char tmp[2];
	char *tmp_result;

	result= ft_strdup("");
	tmp[1] = '\0';
	while (*str)
	{
		if (*str != '\'' && *str != '\"')
		{
			tmp[0] = *str;
			tmp_result = ft_strjoin(result, tmp);
			free(result);
			result = ft_strdup(tmp_result);
			free(tmp_result);
		}
		str++;
	}
	// free(str);
	return result;
}
