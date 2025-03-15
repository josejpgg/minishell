/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:27:25 by jgamarra          #+#    #+#             */
/*   Updated: 2025/03/15 21:17:43 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct cmd *execcmd()
{
  struct execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = EXEC;
  return (struct cmd *)cmd;
}

struct cmd *parseexec(char **ps, char *es)
{
  char *q, *eq;
  int tok, argc;
  struct execcmd *cmd;
  struct cmd *ret;

  //   if(peek(ps, es, "("))
  //     return parseblock(ps, es);
  // printf("ps: %s\n", *ps);
  ret = execcmd();
  cmd = (struct execcmd *)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es);
  while (!peek(ps, es, "|)&;"))
  { // loop character by character
    if ((tok = gettoken(ps, es, &q, &eq)) == 0)
      break;
    if (tok != 'a')
      panic("syntax");
    cmd->argv[argc] = q;
    cmd->eargv[argc] = eq;
    argc++;
    if (argc >= MAXARGS)
      panic("too many args");
    ret = parseredirs(ret, ps, es);
  }
  // print_vector(cmd->argv);
  // print_vector(cmd->eargv);
  cmd->argv[argc] = 0;
  cmd->eargv[argc] = 0;
  return ret;
}

void valid_command(t_execcmd *ecmd, t_minishell *minishell)
{
  if (strchr("./", ecmd->argv[0][0]))
  {
    // #valid if command is executable
    if (access(ecmd->argv[0], F_OK) == -1)
    {
      ft_putstr_fd("minishell: ", 2);
      ft_putstr_fd(ecmd->argv[0], 2);
      ft_putstr_fd(": command not found\n", 2);
      exit(127);
    }
  }
  else
  {
    // loop into minishell->list_path to find the command path
    int i = -1;
    while (minishell->path_env[++i])
    {
      char *command = ft_strjoin("/", ecmd->argv[0]);
      char *full_command = ft_strjoin(minishell->path_env[i], command);
      if (access(full_command, F_OK) == 0)
      {
        ecmd->argv[0] = full_command;
        break;
      }
      free(command);
      free(full_command);
    }
    if (!strchr("./", ecmd->argv[0][0]))
    {
      ft_putstr_fd("minishell: ", 2);
      ft_putstr_fd(ecmd->argv[0], 2);
      ft_putstr_fd(": command not found\n", 2);
      exit(127);
    }
  }
}

struct cmd *parsecmd(char *s)
{
  char *es;
  struct cmd *cmd;

  es = s + strlen(s);                 // es = end of string
  cmd = parseline(&s, es); // s = start of all the string
  peek(&s, es, "");
  if (s != es)
  {
    ft_putstr_fd("leftovers jgg: ", 2);
    ft_putstr_fd(s, 2);
    ft_putchar_fd('\n', 2);
    panic("syntax");
  }
  nulterminate(cmd);
  return cmd;
}

// Execute cmd.  Never returns.
void runcmd(struct cmd *cmd, t_minishell *minishell)
{
  int p[2];
  // struct backcmd *bcmd;
  struct execcmd *ecmd;
  // struct listcmd *lcmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;
	pid_t pid1, pid2;

  if (cmd == 0)
    exit(0);

  switch (cmd->type)
  {
  default:
    panic("runcmd");

  case EXEC:
    ecmd = (struct execcmd *)cmd;
    valid_command(ecmd, minishell);
	print_vector(ecmd->argv);
    if (ecmd->argv[0] == 0)
      exit(0);
	if (valid_builtins(ecmd))
	{
		cmd = prepare_builtins(cmd, minishell);
		run_internal(cmd, minishell);
	}
	else
	{
	    exec_command(ecmd->argv[0], ecmd->argv);
		ft_putstr_fd("exec failed ", 2);
		ft_putstr_fd(ecmd->argv[0], 2);
		ft_putchar_fd('\n', 2);
	}
    break;

  case REDIR:
    rcmd = (struct redircmd *)cmd;
    close(rcmd->fd);
	if (rcmd->mode == O_RDONLY)
	{
		if (open(rcmd->file, rcmd->mode) < 0)
		{
			ft_putstr_fd("open failed ", 2);
			ft_putstr_fd(rcmd->file, 2);
			ft_putchar_fd('\n', 2);
			exit(1);
		}
	}
	else
	{
		if (open(rcmd->file, rcmd->mode, rcmd->right) < 0)
		{
			ft_putstr_fd("open failed ", 2);
			ft_putstr_fd(rcmd->file, 2);
			ft_putchar_fd('\n', 2);
			exit(1);
		}
	}
    runcmd(rcmd->cmd, minishell);
    break;
	
  case PIPE:
    pcmd = (struct pipecmd *)cmd;
    if (pipe(p) < 0)
      panic("pipe");
    if ((pid1 = fork1()) == 0)
    {
      close(1);
    //   dup(p[1]);
		dup2(p[1], 1);
      close(p[0]);
      close(p[1]);
      runcmd(pcmd->left, minishell);
    }
    if ((pid2 = fork1()) == 0)
    {
      close(0);
    //   dup(p[0]);
		dup2(p[0], 0);
      close(p[0]);
      close(p[1]);
      runcmd(pcmd->right, minishell);
    }
    close(p[0]);
    close(p[1]);
    waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
    break;
  }
  exit(0);
}

void exec_command(char *command, char **args)
{
  // ft_printf("%s\n", command);
  // print_vector(args);
  if (execvp(command, args) == -1)
  {
    perror("exec failed");
    exit(EXIT_FAILURE);
  }
}

void control_cmd(t_cmd *cmd, t_minishell *minishell)
{
	if (valid_builtins(cmd))
	{
		cmd = prepare_builtins(cmd, minishell);
		run_internal(cmd, minishell);
	}
	else
	{
		run_external(cmd, minishell);
	}
}
