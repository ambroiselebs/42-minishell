/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:15:12 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 18:05:50 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_return_code = 0;

/**
 * Exécute une liste de commandes
 * Gère le cas où il n'y a qu'une commande (sans pipe)
 * et le cas où il y a plusieurs commandes (avec pipes)
 */
int	execute_commands(t_cmd *cmd_list, t_env **env_list)
{
	int		pipe_count;
	t_cmd	*curr;

	if (!cmd_list)
		return (0);
	pipe_count = 0;
	curr = cmd_list;
	while (curr->next)
	{
		pipe_count++;
		curr = curr->next;
	}
	if (pipe_count == 0)
		return (execute_simple_command(cmd_list, env_list));
	else
		return (execute_pipeline(cmd_list, pipe_count, env_list));
}

/**
 * Exécute une commande simple (sans pipe)
 * Vérifie si c'est un builtin ou une commande externe
 */
int	execute_simple_command(t_cmd *cmd, t_env **env_list)
{
	int	result;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (cmd->redir && !apply_redirections(cmd->redir))
		return (1);
	if (is_builtin(cmd->args[0]))
		result = execute_builtin(cmd, env_list);
	else
		result = execute_external_command(cmd, *env_list);
	return (result);
}

/**
 * Retrouve la valeur d'une variable d'environnement
 * sous forme de tableau pour execve
 */
char	**env_to_array(t_env *env_list)
{
	int		count;
	int		i;
	t_env	*curr;
	char	**env_array;
	char	*tmp;

	count = 0;
	curr = env_list;
	while (curr)
	{
		if (curr->equal_sign)
			count++;
		curr = curr->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	curr = env_list;
	while (curr)
	{
		if (curr->equal_sign)
		{
			tmp = ft_strjoin(curr->name, "=");
			env_array[i++] = ft_strjoin(tmp, curr->value ? curr->value : "");
			free(tmp);
		}
		curr = curr->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

/**
 * Point d'entrée principal pour l'exécution
 * Modifie l'affichage du mode main pour exécuter les commandes
 * au lieu de simplement les afficher
 */
int	exec_shell(t_cmd *cmd, t_env **env)
{
	int	result;

	if (!cmd)
		return (0);
	// Configurer les gestionnaires de signaux pour l'exécution
	setup_signals_exec();
	// Exécuter les commandes
	result = execute_commands(cmd, env);
	// Restaurer les gestionnaires de signaux par défaut
	setup_signals_shell();
	return (result);
}
