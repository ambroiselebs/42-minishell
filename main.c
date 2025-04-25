/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 19:23:01 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 17:59:09 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Lit l'entrée utilisateur et retourne la ligne saisie
 */
static char	*get_input(void)
{
	char	*input;

	input = custom_reader();
	if (input)
		add_history(input);
	return (input);
}

/**
 * Traite l'entrée utilisateur : tokenize, parse et exécute
 */
static void	process_input(char *input, t_env **env)
{
	t_token	*tokens;
	t_cmd	*cmd;

	if (!check_input(input) || !(*input))
		return ;
	tokens = tokenize(input, *env);
	cmd = parse(tokens, env);
	if (cmd)
	{
		if (prepare_execution(cmd))
			exec_shell(cmd, env);
	}
	clean_shell(input, tokens, cmd);
}

/**
 * Initialise le shell (signaux, terminal, etc.)
 */
static void	init_shell(void)
{
	setup_signals_shell();
	rl_catch_signals = 0;
}

/**
 * Nettoie le shell avant de quitter
 */
static void	cleanup_shell(t_env *env)
{
	free_env(env);
	rl_clear_history();
}

/**
 * Fonction principale du shell
 */
int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*input;
	int		exit_status;

	(void) argc;
	(void) argv;
	env = NULL;
	env_init(&env, envp);
	init_shell();
	exit_status = 0;
	while (!exit_status)
	{
		input = get_input();
		if (!input)
			break ;
		if (*input)
			process_input(input, &env);
	}
	cleanup_shell(env);
	ft_putendl_fd("exit", 1);
	return (g_return_code);
}
