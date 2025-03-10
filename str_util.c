/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 19:14:24 by jgamarra          #+#    #+#             */
/*   Updated: 2025/01/26 16:39:51 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcountchr(char *str, char chr)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (str[++i])
	{
		if (str[i] == chr)
			count++;
	}
	return (count);
}

/*
* Function to clean input param from spaces
* ' ' '\t' '\n' '\v' '\f' '\r'
* Return the cleaned string
*/
char	*trim_space_char(char *input)
{
	int		index;
	char	*tmp;
	char	*spaces;

	index = 0;
	spaces = " \t\n\v\f\r";
	while ((ft_isspace(input[0]) || ft_isspace(input[ft_strlen(input) - 1]))
		&& index <= 5)
	{
		tmp = ft_strtrim(input, &spaces[index]);
		free(input);
		input = tmp;
		index++;
	}
	return (input);
}
