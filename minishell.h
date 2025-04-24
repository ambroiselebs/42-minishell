/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:18:27 by aberenge          #+#    #+#             */
/*   Updated: 2025/04/24 17:25:06 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef struct s_redir
{
	char	*file;
	int		type;
	t_redir	*next;
}	t_redir;


typedef struct s_env
{
	char	*name;
	char	*value;
	int		equal_sign;
	t_env	*next;
}	t_env;

typedef struct s_token
{
	char	*value;
	int		type;
	t_token	*next;
}	t_token;

typedef struct s_cmd
{
	char	*name;
	char	*path;
	char	**args;
}	t_cmd;
