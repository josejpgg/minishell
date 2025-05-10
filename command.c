/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:27:25 by jgamarra          #+#    #+#             */
/*   Updated: 2025/05/10 14:25:57 by jgamarra         ###   ########.fr       */
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

  ret = execcmd();
  cmd = (struct execcmd *)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es);

  while (!peek(ps, es, "|"))
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
	// printf("tok: %d\n", tok);
  }
//   print_vector(cmd->argv);
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

  es = s + strlen(s);      // es = end of string
  cmd = parseline(&s, es); // s = start of all the string
  peek(&s, es, "");
  if (s != es)
  {
    ft_putstr_fd("leftovers: ", 2);
    ft_putstr_fd(s, 2);
    ft_putchar_fd('\n', 2);
    panic("syntax");
  }
  
  nulterminate(cmd);
  return cmd;
}

char *read_stdin_to_str(void)
{
	char buffer[1024];
	char *result = malloc(1);
	ssize_t bytes_read;
	size_t total = 0;

	if (!result)
		return (NULL);
	result[0] = '\0';

	while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
	{
		char *tmp = malloc(total + bytes_read + 1);
		if (!tmp)
			return (NULL);
		for (size_t i = 0; i < total; i++)
			tmp[i] = result[i];
		for (ssize_t j = 0; j < bytes_read; j++)
			tmp[total + j] = buffer[j];
		tmp[total + bytes_read] = '\0';
		free(result);
		result = tmp;
		total += bytes_read;
	}
	return result;
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
    if (ecmd->argv[0] == 0)
      exit(0);
	if (valid_builtins(cmd))
	{
    // printf("check this\n");
    // char *tmp = read_stdin_to_str();
    // if (tmp)
    // {
    //   ecmd->argv[1] = tmp;
    //   ecmd->argv[2] = 0;
    //   // free(tmp);
    // }
		run_internal(cmd, minishell);
	}
	else
	{
    valid_command(ecmd, minishell);
    int idx = 1;
    while (ecmd->argv[idx])
    {
        ecmd->argv[idx] = expand_variables(ecmd->argv[idx], minishell);
        remove_quotes(ecmd, idx);
        idx++;
    }
    exec_command(ecmd->argv[0], ecmd->argv);
		ft_putstr_fd("exec failed ", 2);
		ft_putstr_fd(ecmd->argv[0], 2);
		ft_putchar_fd('\n', 2);
	}
    break;

  case REDIR:
    rcmd = (struct redircmd *)cmd;
    // if (rcmd->hdoc) {
    //     int pipefd[2];
    //     if (pipe(pipefd) < 0)
    //         panic("pipe error");
    //     if (fork1() == 0) {
    //         close(pipefd[0]);
    //         char *line = NULL;
    //         while ((line = get_next_line(STDIN_FILENO))) {
    //           printf("test!\n");
    //             size_t length = ft_strlen(line);
    //             if (length > 0 && line[length - 1] == '\n')
    //                 line[length - 1] = '\0';
    //             if (strcmp(line, rcmd->hdoc) == 0) {
    //                 free(line);
    //                 break;
    //             }
    //             write(pipefd[1], line, ft_strlen(line));
    //             write(pipefd[1], "\n", 1);
    //             free(line);
    //         }
    //         close(pipefd[1]);
    //         exit(0);
    //     }
    //     close(pipefd[1]);
    //     dup2(pipefd[0], STDIN_FILENO);
    //     close(pipefd[0]);
    // }
    if (rcmd->hdoc) {
        int pipefd[2];
        if (pipe(pipefd) < 0)
            panic("pipe error");
        // write heredoc content to pipe
        write(pipefd[1], rcmd->hdoc, ft_strlen(rcmd->hdoc));
        close(pipefd[1]);

        // redirect stdin
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        free(rcmd->hdoc);

        // ecmd = (struct execcmd *)cmd;
        // ecmd->argv[1] = rcmd->hdoc;
        // ecmd->argv[2] = 0;
        
    }
     else {
        int fd;
        if (rcmd->mode == O_RDONLY) {
            fd = open(rcmd->file, O_RDONLY);
            if (fd < 0) {
                perror("open redirection failed");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        } else if (rcmd->mode == (O_WRONLY | O_CREAT | O_TRUNC)) {
            fd = open(rcmd->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open redirection failed");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (rcmd->mode == (O_WRONLY | O_CREAT | O_APPEND)) {
            fd = open(rcmd->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) {
                perror("open redirection failed");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else {
            perror("unknown redirection mode");
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

// void exec_command(char *command, char **args)
// {
//     // arrancamos en args+1 para no tocar el nombre del comando
//     char **p = args + 1;
//     while (*p)
//     {
//         char *clean = remove_quotes_simple(*p);
//         // NO free(*p);
//         *p = clean;
//         p++;
//     }
//     if (execvp(command, args) == -1)
//     {
//         perror("exec failed");
//         exit(EXIT_FAILURE);
//     }
// }

// cat archivo.txt | grep hola | wc
// cat archivo.txt | grep "hola" | wc
// cat archivo.txt | grep 'hola' | wc

void control_cmd(t_cmd *cmd, t_minishell *minishell)
{
	if (valid_builtins(cmd))
	{
		run_internal(cmd, minishell);
	}
	else
	{
		run_external(cmd, minishell);
	}
}
