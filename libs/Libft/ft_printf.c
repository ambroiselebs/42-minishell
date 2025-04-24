/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberenge <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-12 22:20:50 by aberenge          #+#    #+#             */
/*   Updated: 2024-12-12 22:20:50 by aberenge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_parser(char c, int *counter, va_list *args)
{
	if (c == 'c')
		*counter += ft_putchar_fd(va_arg(*args, int), 1);
	if (c == 's')
		*counter += ft_putstr_fd(va_arg(*args, char *), 1);
	if (c == 'p')
		*counter += ft_print_pointer(va_arg(*args, unsigned long long), 1);
	if (c == 'i' || c == 'd')
		*counter += ft_putnbr_fd(va_arg(*args, int), 1);
	if (c == 'u')
		*counter += ft_putnbr_unsigned_fd(va_arg(*args, unsigned int), 1);
	if (c == 'x')
		*counter += ft_print_hex_fd_min(va_arg(*args, int), 1);
	if (c == 'X')
		*counter += ft_print_hex_fd_maj(va_arg(*args, int), 1);
	if (c == '%')
		*counter += ft_putchar_fd('%', 1);
}

int	ft_printf(const char *str, ...)
{
	int		counter;
	va_list	args;

	counter = 0;
	va_start(args, str);
	while (*str)
	{
		if (*str == '%')
			ft_parser(*++str, &counter, &args);
		else
			counter += ft_putchar_fd(str[0], 1);
		str++;
	}
	va_end(args);
	return (counter);
}
