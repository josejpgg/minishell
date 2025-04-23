/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:29:04 by jgamarra          #+#    #+#             */
/*   Updated: 2025/04/23 22:32:12 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct cmd* redircmd(struct cmd *subcmd, char *file, char *efile, int mode, mode_t right, int fd, char *hdoc) {
    struct redircmd *cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = REDIR;
    cmd->cmd = subcmd;
    cmd->file = file;
    cmd->efile = efile;
    cmd->mode = mode;
    cmd->right = right;
    cmd->fd = fd;
    cmd->hdoc = hdoc;
    return (struct cmd*)cmd;
}


char *process_heredoc(char *q, char *eq) {
    char *heredoc = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    FILE *input = stdin;
    char delimiter[eq - q + 1];
    strncpy(delimiter, q, eq - q);
    delimiter[eq - q] = '\0';
    printf("HEREDOC START: Delimiter is '%s'\n", delimiter);
    heredoc = malloc(1);
    if (!heredoc)
        panic("Memory allocation failed");
    *heredoc = '\0';
    while ((nread = getline(&line, &len, input)) != -1) {
        if (strncmp(line, delimiter, strlen(delimiter)) == 0 && line[strlen(delimiter)] == '\n')
            break;
        heredoc = realloc(heredoc, strlen(heredoc) + nread + 1);
        strcat(heredoc, line);
    }
    free(line);
    printf("%s", heredoc);
    return heredoc;
}

struct cmd* parseredirs(struct cmd *cmd, char **ps, char *es)
{
  int tok;
  char *q, *eq;

  while(peek(ps, es, "<>")){
    tok = gettoken(ps, es, 0, 0);
    if(gettoken(ps, es, &q, &eq) != 'a')
      panic("missing file for redirection");
    switch(tok){
    case '<':
      cmd = redircmd(cmd, q, eq, O_RDONLY, 0, 0);
      break;
    case '>':
      cmd = redircmd(cmd, q, eq, O_CREAT|O_WRONLY|O_TRUNC, 0644, 1);
      break;
    case '+':  // >>
      cmd = redircmd(cmd, q, eq, O_CREAT|O_WRONLY|O_APPEND, 0644, 1);
      break;
    // << heredoc to -
    }
  }
  return cmd;
}

