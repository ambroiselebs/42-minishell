/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:40:12 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 17:57:04 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Libère un tableau de chaînes de caractères
 */
void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/**
 * Trouve le nombre maximum de descripteurs de fichiers ouverts
 */
int	get_max_fd(void)
{
	int	max_fd;

	max_fd = sysconf(_SC_OPEN_MAX);
	if (max_fd < 0)
		max_fd = 1024;
	return (max_fd);
}

/**
 * Ferme tous les descripteurs de fichiers sauf stdin, stdout et stderr
 */
void	close_all_fds(void)
{
	int	i;
	int	max_fd;

	max_fd = get_max_fd();
	i = 3;
	while (i < max_fd)
	{
		close(i);
		i++;
	}
}

/**
 * Cherche un exécutable dans les chemins PATH
 * et vérifie les permissions
 */
char	*validate_executable(char *cmd_path)
{
	struct stat	st;

	if (!cmd_path)
		return (NULL);
	if (stat(cmd_path, &st) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (NULL);
	}
	if (S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_path, 2);
		ft_putstr_fd(": is a directory\n", 2);
		return (NULL);
	}
	if (!(st.st_mode & S_IXUSR))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (NULL);
	}
	return (cmd_path);
}

/**
 * Prépare le shell pour l'exécution d'une commande
 * Configure les redirections et les heredocs
 */
int	prepare_execution(t_cmd *cmd_list)
{
	if (!cmd_list)
		return (0);
	if (!prepare_all_heredocs(cmd_list))
		return (0);
	return (1);
}
