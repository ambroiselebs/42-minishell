/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:25:12 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 17:56:29 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Crée les pipes nécessaires pour l'exécution de plusieurs commandes
 */
int	create_pipes(int **pipes, int pipe_count)
{
	int	i;

	*pipes = malloc(sizeof(int) * pipe_count * 2);
	if (!*pipes)
		return (0);
	i = 0;
	while (i < pipe_count)
	{
		if (pipe(&(*pipes)[i * 2]) == -1)
		{
			while (--i >= 0)
			{
				close((*pipes)[i * 2]);
				close((*pipes)[i * 2 + 1]);
			}
			free(*pipes);
			return (0);
		}
		i++;
	}
	return (1);
}

/**
 * Configure les descripteurs de fichiers pour les pipes
 * selon la position de la commande dans le pipeline
 */
void	setup_pipes(int *pipes, int i)
{
	if (i > 0)
	{
		dup2(pipes[(i - 1) * 2], STDIN_FILENO);
	}
	if (pipes[i * 2 + 1])
	{
		dup2(pipes[i * 2 + 1], STDOUT_FILENO);
	}
	close_all_pipes(pipes);
}

/**
 * Ferme tous les descripteurs de fichiers des pipes
 */
void	close_all_pipes(int *pipes)
{
	int	i;

	i = 0;
	while (pipes[i] != -1)
	{
		close(pipes[i]);
		i++;
	}
}

/**
 * Attend la fin de tous les processus fils
 * et récupère le code de retour du dernier
 */
void	wait_for_children(pid_t *pids, int count)
{
	int	i;
	int	status;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (i == count - 1)
		{
			if (WIFEXITED(status))
				g_return_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_return_code = 128 + WTERMSIG(status);
		}
		i++;
	}
}

/**
 * Exécute une séquence de commandes connectées par des pipes
 */
int	execute_pipeline(t_cmd *cmd_list, int pipe_count, t_env **env_list)
{
	pid_t	*pids;
	int		*pipes;
	int		i;
	t_cmd	*curr;

	if (!create_pipes(&pipes, pipe_count))
		return (print_error("pipe", strerror(errno)));
	pipes[pipe_count * 2] = -1;  // Marqueur de fin
	pids = malloc(sizeof(pid_t) * (pipe_count + 1));
	if (!pids)
		return (free(pipes), print_error("malloc", strerror(errno)));
	i = 0;
	curr = cmd_list;
	while (curr)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			return (free(pipes), free(pids),
				print_error("fork", strerror(errno)));
		if (pids[i] == 0)
			child_process(curr, env_list, pipes, i);
		curr = curr->next;
		i++;
	}
	close_all_pipes(pipes);
	wait_for_children(pids, pipe_count + 1);
	free(pipes);
	free(pids);
	return (g_return_code);
}
