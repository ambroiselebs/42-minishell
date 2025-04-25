/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:20:12 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 18:03:31 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Exécute une commande externe en créant un processus fils
 */
int	execute_external_command(t_cmd *cmd, t_env *env_list)
{
	pid_t	pid;
	int		status;
	char	**env_array;

	pid = fork();
	if (pid == -1)
		return (print_error("fork", strerror(errno)));
	if (pid == 0)
	{
		// Processus fils
		env_array = env_to_array(env_list);
		if (!env_array)
			exit(1);
		if (execve(cmd->path, cmd->args, env_array) == -1)
		{
			free_array(env_array);
			exit(127);
		}
	}
	// Processus parent
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_return_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_return_code = 128 + WTERMSIG(status);
	return (g_return_code);
}

/**
 * Exécute une commande builtin
 * Retourne le code de retour
 */
int	execute_builtin(t_cmd *cmd, t_env **env_list)
{
	(void) cmd;
	(void) env_list;
	/* if (ft_strcmp(cmd->args[0], "echo") == 0)
		g_return_code = ft_echo(cmd->args);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		g_return_code = ft_cd(cmd->args, env_list);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		g_return_code = ft_pwd();
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		g_return_code = ft_export(cmd->args, env_list);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		g_return_code = ft_unset(cmd->args, env_list);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		g_return_code = ft_env(*env_list);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		g_return_code = ft_exit(cmd->args);
	return (g_return_code); */
	return (1);
}

/**
 * Exécute une commande builtin dans un processus fils
 * Utilisé lorsqu'il y a des pipes
 */
void	execute_forked_builtin(t_cmd *cmd, t_env **env_list)
{
	int	result;

	result = execute_builtin(cmd, env_list);
	exit(result);
}

/**
 * Lance l'exécution d'une commande dans un processus fils
 * Gère les redirections et les builtins
 */
void	child_process(t_cmd *cmd, t_env **env_list, int *pipes, int i)
{
	char	**env_array;

	setup_pipes(pipes, i);
	if (cmd->redir && !apply_redirections(cmd->redir))
		exit(1);
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
		execute_forked_builtin(cmd, env_list);
	env_array = env_to_array(*env_list);
	if (!env_array)
		exit(1);
	if (execve(cmd->path, cmd->args, env_array) == -1)
	{
		free_array(env_array);
		if (access(cmd->path, F_OK) == -1)
			exit(127); // Command not found
		else
			exit(126); // Permission denied
	}
}

/**
 * Affiche un message d'erreur et retourne un code d'erreur
 */
int	print_error(char *cmd, char *error)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\n", 2);
	return (1);
}
