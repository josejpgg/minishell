/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 19:44:37 by jgamarra          #+#    #+#             */
/*   Updated: 2025/06/07 13:37:21 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void handle_code(int ms_error, int *error_code)
{
	if (ms_error == IS_DIR)
		*error_code = 126;
	else if (ms_error == NO_FILE)
		*error_code = 127;
	else if (ms_error == CMD_NOT_FOUND)
		*error_code = 127;
	else if (ms_error == NO_PERM)
		*error_code = 126;
	else
		*error_code = 1;
}

static void handle_msg(int ms_error, int *error_code)
{
	handle_code(ms_error, error_code);
	if (ms_error == IS_DIR)
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
	else if (ms_error == NO_FILE)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else if (ms_error == CMD_NOT_FOUND)
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	else if (ms_error == NO_PERM)
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	else
		ft_putstr_fd(": Unknown error\n", STDERR_FILENO);
}

int handle_error(char *cmd)
{
	int error_code;

	error_code = 1;
	ft_putstr_fd(PROMPT_ERROR, STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (opendir(cmd) != NULL)
		handle_msg(IS_DIR, &error_code);
	else if (cmd[0] == '/')
		handle_msg(NO_FILE, &error_code);
	else
		handle_msg(CMD_NOT_FOUND, &error_code);
	return (error_code);
}

int handle_access_error(char *cmd)
{
	struct stat	path_stat;
	int error_code;

	error_code = 1;
	ft_putstr_fd(PROMPT_ERROR, STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (access(cmd, F_OK) == -1)
		handle_msg(NO_FILE, &error_code);
	else if (access(cmd, R_OK) == -1)
		handle_msg(NO_PERM, &error_code);
	else if (access(cmd, W_OK) == -1 && access(cmd, F_OK) == 0)
		handle_msg(NO_PERM, &error_code);
	else if (access(cmd, X_OK) == -1)
		handle_msg(NO_PERM, &error_code);
	else if (stat(cmd, &path_stat) == 0
		&& (S_ISDIR(path_stat.st_mode)) == 1)
		handle_msg(IS_DIR, &error_code);
	else
		handle_msg(-1, &error_code);
	return (error_code);
}
