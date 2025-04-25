/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:35:12 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 17:56:49 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Gestionnaire pour SIGINT (Ctrl+C) en mode shell
 */
static void	sigint_handler_shell(int sig)
{
	(void)sig;
	g_return_code = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * Gestionnaire pour SIGINT (Ctrl+C) en mode heredoc
 */
static void	sigint_handler_heredoc(int sig)
{
	(void)sig;
	g_return_code = 130;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

/**
 * Gestionnaire pour SIGINT (Ctrl+C) en mode exécution
 */
static void	sigint_handler_exec(int sig)
{
	(void)sig;
	g_return_code = 130;
	write(STDOUT_FILENO, "\n", 1);
}

/**
 * Configure les gestionnaires de signaux pour le mode shell
 */
void	setup_signals_shell(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sigint_handler_shell;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

/**
 * Configure les gestionnaires de signaux pour le mode heredoc
 */
void	setup_signals_heredoc(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sigint_handler_heredoc;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

/**
 * Configure les gestionnaires de signaux pour le mode exécution
 */
void	setup_signals_exec(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sigint_handler_exec;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

/**
 * Configure les gestionnaires de signaux pour les processus fils
 */
void	setup_signals_child(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
