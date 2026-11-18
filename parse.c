/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmontezu <tmontezu@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 18:13:27 by tmontezu          #+#    #+#             */
/*   Updated: 2026/11/18 18:56:49 by tmontezu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

int is_valid_number(char *str)
{
	int i;
	i = 0;

	if (!str || str[i] == '\0')
		return 0;

	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return 0;
		
		i++;
	}
	return 1;
}

int is_max(char *str)
{
	int len;
	int i;

	i = 0;
	if (!str)
		return (0);
	
	while (str[i] == '0')
		i++;

	if (str[i] == '\0')
		return(1);
	
	len = strlen(&str[i]);
	if (len > 10)
		return (0);
	if (len < 10)
		return (1);
	
	if (strcmp(&str[i], "2147483647") > 0)
		return (0);

	return (1);
}
