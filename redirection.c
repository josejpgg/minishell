/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:29:04 by jgamarra          #+#    #+#             */
/*   Updated: 2025/05/23 19:38:40 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *process_heredoc2(char *q, char *eq)
{
    char *heredoc = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    FILE *input = stdin;
    char delimiter[eq - q + 1];
    strncpy(delimiter, q, eq - q);
    delimiter[eq - q] = '\0';
    char *str_delimiter = ft_strdup(delimiter);
    str_delimiter = remove_quotes_simple(str_delimiter);
    heredoc = malloc(1);
    if (!heredoc)
        panic("Memory allocation failed");
    *heredoc = '\0';
    while (1)
    {
        line = readline("> ");
        if (line == NULL)
            break ;
        line = realloc(line, strlen(line) + 2);
        strcat(line, "\n");
        if (strncmp(line, str_delimiter, strlen(str_delimiter)) == 0 && line[strlen(str_delimiter)] == '\n')
            break ;
        heredoc = realloc(heredoc, strlen(heredoc) + strlen(line) + 1);
        strcat(heredoc, line);
        free(line);
    }
    free(line);
    return heredoc;
}
