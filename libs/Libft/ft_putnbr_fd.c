/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge & mpapin <marvin@42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 13:20:42 by aberenge          #+#    #+#             */
/*   Updated: 2024/10/17 16:14:52 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd(int nb, int fd)
{
	int		counter;
	char	c;

	counter = 0;
	if (nb == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return (11);
	}
	if (nb < 0)
	{
		write(fd, "-", 1);
		nb = -nb;
		counter++;
	}
	if (nb >= 10)
	{
		counter += ft_putnbr_fd(nb / 10, fd);
	}
	c = (nb % 10) + '0';
	write(fd, &c, 1);
	counter++;
	return (counter);
}
