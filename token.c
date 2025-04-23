/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:25:10 by jgamarra          #+#    #+#             */
/*   Updated: 2025/04/23 22:22:20 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>";
char quotes[] = "'\"";

int	gettoken(char **ps, char *es, char **q, char **eq)
{
  char *s;
  int ret;
  char c;

  s = *ps;
  while(s < es && strchr(whitespace, *s))
    s++;
  if(q)
    *q = s;
  ret = *s;
  switch(*s){
  case 0:
    break;
  case '|':
  case '<':
    s++;
    if (s < es && *s == '<') {
        ret = HDOC;
        s++;
    }
    break;
  case '>':
    s++;
    if(*s == '>'){
      ret = '+';
      s++;
    }
    break;
  case '"':
  case '\'':
    ret = *s;
    s++;
    while(s < es && *s != ret)
    {
      s++;
    }
    if(s < es)
      s++;
    ret = 'a';
    while(s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
    {
      if (*s == '\'' || *s == '"')
      {
        c = *s;
        s++;
        while(s < es && *s != c)
          s++;
        if(s < es)
          s++;
      }
      else
        s++;
    }
    break;
  default:
    ret = 'a';
    while(s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
    {
      if (*s == '\'' || *s == '"')
      {
        c = *s;
        s++;
        while(s < es && *s != c)
          s++;
        if(s < es)
          s++;
      }
      else
        s++;
    }
    break;
  }
  if(eq)
    *eq = s;

  while(s < es && strchr(whitespace, *s))
    s++;
  *ps = s;
  return ret;
}

int	peek(char **ps, char *es, char *toks)
{
  char *s;

  s = *ps;
  while(s < es && strchr(" \t\r\n\v", *s))
    s++;
  *ps = s;
  return *s && strchr(toks, *s);
}
