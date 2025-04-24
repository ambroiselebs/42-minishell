/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge & mpapin <marvin@42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 18:15:49 by aberenge          #+#    #+#             */
/*   Updated: 2024/10/14 16:39:28 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *str, char const *sep)
{
	int	count;

	count = 0;
	while (*str)
	{
		while (*str && ft_strnstr(str, sep, ft_strlen(sep)) == str)
			str += ft_strlen(sep);
		if (*str)
			count++;
		while (*str && ft_strnstr(str, sep, ft_strlen(sep)) != str)
			str++;
	}
	return (count);
}

static char	*malloc_word(char const *str, char const *sep)
{
	int		len;
	char	*word;

	len = 0;
	while (str[len] && ft_strnstr(&str[len], sep, ft_strlen(sep)) != &str[len])
		len++;
	word = (char *) malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	word[len] = '\0';
	while (len--)
		word[len] = str[len];
	return (word);
}

char	**ft_split(char const *s, char const *sep)
{
	int		i;
	int		words;
	char	**tab;

	i = 0;
	words = count_words(s, sep);
	tab = (char **) malloc(sizeof(char *) * (words + 1));
	if (!tab)
		return (NULL);
	while (*s)
	{
		while (*s && ft_strnstr(s, sep, ft_strlen(sep)) == s)
			s += ft_strlen(sep);
		if (*s)
		{
			tab[i++] = malloc_word(s, sep);
			while (*s && ft_strnstr(s, sep, ft_strlen(sep)) != s)
				s++;
		}
	}
	tab[i] = NULL;
	return (tab);
}
