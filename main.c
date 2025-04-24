/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:23:01 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/24 19:31:49 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_input(void)
{
	char	*input;

	input = readline("$> ");
	return (input);
}

int main(int argc, char **argv, char **env)
{
	char	*input;
	int		exit_status;

	(void) argc;
	(void) argv;
	(void) env;
	exit_status = 0;
	while (!exit_status)
	{
		input = get_input();
		if (!input)
			break ;
		if (*input)
			printf("%s\n", input);
		free(input);
	}
	return (0);
}
