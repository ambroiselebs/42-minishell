/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_unsigned_fd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-12 22:23:19 by aberenge          #+#    #+#             */
/*   Updated: 2024-12-12 22:23:19 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_unsigned_fd(unsigned int n, int fd)
{
	int	counter;

	counter = 0;
	if (n >= 10)
		counter += ft_putnbr_unsigned_fd(n / 10, fd);
	ft_putchar_fd((n % 10) + '0', fd);
	counter++;
	return (counter);
}
