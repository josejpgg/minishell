/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 19:33:31 by jgamarra          #+#    #+#             */
/*   Updated: 2025/05/23 19:38:27 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* Function to parse the command and its arguments
* and return a command structure.
*/
static t_cmd	*parse_exec_args(t_cmd *ret, char **ps, char *es, t_minishell *minishell)
{
	char		*q;
	char		*eq;
	int			tok;
	t_execcmd	*cmd;
	int			argc;

	cmd = (t_execcmd *)ret;
	argc = 0;
	while (!peek(ps, es, "|") && !minishell->error_syntax)
	{
		tok = gettoken(ps, es, &q, &eq);
		if (tok == 0)
			break ;
		if (tok != 'a')
			panic("syntax");
		if (argc >= MAXARGS)
			panic("too many args");
		cmd->argv[argc] = q;
		cmd->eargv[argc] = eq;
		argc++;
		ret = parseredirs(ret, ps, es, minishell);
	}
	cmd->argv[argc] = 0;
	cmd->eargv[argc] = 0;
	return (ret);
}

t_cmd	*parseredirs(struct cmd *cmd, char **ps, char *es, t_minishell *minishell)
{
    int		tok;
    char	*q;
	char	*eq;
    char	*hdoc;
	
	hdoc = NULL;
    while (peek(ps, es, "<>") && !minishell->error_syntax)
	{
        tok = gettoken(ps, es, &q, &eq);
		if (gettoken(ps, es, &q, &eq) != 'a')
		{
			panic("syntax error near unexpected token.\n");
			minishell->status = 258;
			minishell->error_syntax = true;
			break ;
		}
        if (tok == HDOC)
		{
            // hdoc = process_heredoc2(q, eq);
            hdoc = process_heredoc(q, eq);
            cmd = redircmd_hdoc(cmd, hdoc);
        }
		else
		{
            if (tok == '<' || tok == '>' || tok == '+')
			{
				cmd = redircmd(cmd, q, eq, tok);
				break ;
			}
        }
    }
    return cmd;
}

/*
* Function to parse a command string by redirections.
*/
t_cmd	*parseexec(char **ps, char *es, t_minishell *minishell)
{
	t_cmd	*ret;

	ret = execcmd();
	ret = parseredirs(ret, ps, es, minishell);
	ret = parse_exec_args(ret, ps, es, minishell);
	return (ret);
}

/*
* Function to parse a command string by pipes.
*/
t_cmd	*parsepipe(char **ps, char *es, t_minishell *minishell)
{
	t_cmd	*cmd;

	cmd = parseexec(ps, es, minishell);
	if (peek(ps, es, "|") && !minishell->error_syntax)
	{
		gettoken(ps, es, 0, 0);
		cmd = pipecmd(cmd, parsepipe(ps, es, minishell));
	}
	return (cmd);
}

/*
* Function to parse a command string into a command structure by tokenizing
* the output is a structure that contains the command and its arguments.
*/
t_cmd	*parsecmd(char *s, t_minishell *minishell)
{
	char		*es;
	struct cmd	*cmd;

	es = s + ft_strlen(s);
	cmd = parsepipe(&s, es, minishell);
	if (!minishell->error_syntax)
	{
		peek(&s, es, "");
		if (s != es)
		{
			ft_putstr_fd("leftovers: ", 2);
			ft_putstr_fd(s, 2);
			ft_putchar_fd('\n', 2);
			panic("syntax");
		}
		nulterminate(cmd);
	}
	return (cmd);
}
