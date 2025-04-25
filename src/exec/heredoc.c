/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:30:12 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/25 18:04:01 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Crée un nom de fichier temporaire pour le heredoc
 * Format: /tmp/minishell_heredoc_XXXX où XXXX est un nombre aléatoire
 */
static char	*create_heredoc_name(void)
{
	char	*name;
	int		fd;
	int		random;

	name = malloc(sizeof(char) * 30);
	if (!name)
		return (NULL);
	ft_strcpy(name, "/tmp/minishell_heredoc_");
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
	{
		free(name);
		return (NULL);
	}
	read(fd, &random, sizeof(int));
	close(fd);
	if (random < 0)
		random = -random;
	random = random % 10000;
	name[22] = '0' + (random / 1000) % 10;
	name[23] = '0' + (random / 100) % 10;
	name[24] = '0' + (random / 10) % 10;
	name[25] = '0' + random % 10;
	name[26] = '\0';
	return (name);
}

/**
 * Lit les entrées de l'utilisateur jusqu'à ce que le délimiteur soit trouvé
 * Écrit ces entrées dans le fichier temporaire
 */
static int	read_heredoc_input(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (1);
}

/**
 * Traite un heredoc en créant un fichier temporaire et en lisant l'entrée
 * Retourne le nom du fichier temporaire ou NULL en cas d'erreur
 */
char	*process_heredoc(char *delimiter)
{
	char	*filename;
	int		fd;
	int		result;

	filename = create_heredoc_name();
	if (!filename)
		return (NULL);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(filename);
		return (NULL);
	}
	result = read_heredoc_input(fd, delimiter);
	close(fd);
	if (!result)
	{
		unlink(filename);
		free(filename);
		return (NULL);
	}
	return (filename);
}

/**
 * Prépare tous les heredocs d'une commande
 * Remplace les fichiers des redirections HEREDOC par les fichiers temporaires
 */
int	prepare_heredocs(t_cmd *cmd)
{
	t_redir	*curr;
	char	*heredoc_file;

	if (!cmd || !cmd->redir)
		return (1);
	curr = cmd->redir;
	while (curr)
	{
		if (curr->type == HEREDOC)
		{
			heredoc_file = process_heredoc(curr->file);
			if (!heredoc_file)
				return (0);
			free(curr->file);
			curr->file = heredoc_file;
			curr->type = REDIR_IN;
		}
		curr = curr->next;
	}
	return (1);
}

/**
 * Prépare tous les heredocs de toutes les commandes
 */
int	prepare_all_heredocs(t_cmd *cmd_list)
{
	t_cmd	*curr;

	curr = cmd_list;
	while (curr)
	{
		if (!prepare_heredocs(curr))
			return (0);
		curr = curr->next;
	}
	return (1);
}
