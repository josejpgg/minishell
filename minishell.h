/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 18:21:09 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/12 21:50:54 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define PROMPT "minishell$ "
# define MAXARGS 10

# include "lib/libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <signal.h>
// LINUX
// # include <linux/limits.h> 
// MAC
# include <limits.h>
# include <stdbool.h>
# include <sys/types.h>

typedef enum e_type
{
	EXEC,
	REDIR,
	PIPE,
	HDOC,
}	t_type;

typedef struct cmd
{
  t_type type;
}	t_cmd;

typedef struct execcmd
{
	t_type type;
	char *argv[MAXARGS];
	char *eargv[MAXARGS];
}	t_execcmd;

typedef struct pipecmd
{
	int type;
	t_cmd *left;
	t_cmd *right;
}	t_pipecmd;

typedef struct redircmd
{
	int type;
	t_cmd *cmd;
	char *file;
	char *efile;
	int mode;
	mode_t right;
	int fd;
}	t_redircmd;

typedef enum e_response_msg
{
	CMD_NOT_FOUND,
	NO_SUCH_FILE,
	PERM_DENIED,
	AMBIGUOUS,
	TOO_MANY_ARGS,
	NUMERIC_REQUI
}	t_response_msg;

typedef enum e_response_code
{
	SUCCESS,
	GENERAL,
	CANT_EXEC = 126,
	NOT_FOUND,
	EXEC_255 = 255
}	t_response_code;

typedef struct s_response
{
	t_response_code	no;
	t_response_msg	msg;
	char		*cause;
}	t_response;

typedef struct minishell
{
	char	**path_env;
	char	**env;
}	t_minishell;

// env
void		init_env(t_minishell *minishell, char **envp);
int			ft_env(char **envp, char *key, char *value);

//params
void	valid_inital_param(int argc, char **envp, t_minishell *minishell);

//safe func
void	*safe_malloc(size_t size);
pid_t	safe_fork(void);
int safe_open(char *file, int flags, mode_t mode);
void safe_pipe(int *pipefd);
// void	safe_execve(t_minishell *minishell, char **argv);

// interactive
void	catch_signal(void);
void	catch_interactive(char *input, char *prompt);
void	save_history(char *input);
char	*check_input_valid(char *input);

// safe_free.c
void	safe_free_vector(char **split);
void	safe_free_minishell(t_minishell *minishell);

// str_util
int ft_strcountchr(char *str, char chr);
char	*trim_space_char(char *input);

// token.c
int	gettoken(char **ps, char *es, char **q, char **eq);
int	peek(char **ps, char *es, char *toks);

// command.c
struct cmd* execcmd(void);
struct cmd* parseexec(char **ps, char *es);
struct cmd* parsecmd(char *s);
void runcmd(struct cmd *cmd);
void exec_command(char *command, char **args);

// redirection.c
struct cmd* redircmd(struct cmd *subcmd, char *file, char *efile, int mode, mode_t right, int fd);
struct cmd* parseredirs(struct cmd *cmd, char **ps, char *es);

// pipe.c
struct cmd* pipecmd(struct cmd *left, struct cmd *right);
struct cmd* parsepipe(char **ps, char *es);
struct cmd* parseline(char **ps, char *es);

// line.c
struct cmd* nulterminate(struct cmd *cmd);
void panic(char *s);
int fork1(void);

#endif
