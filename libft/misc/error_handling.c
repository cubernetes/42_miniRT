/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:02:12 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/17 03:41:28 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdlib.h>
#include <execinfo.h>

#define MAX_CALLSTACK_SIZE 128

/* Assumes a format like this: */
/* /usr/lib/libc.so.6(__libc_start_main+0x8a) [0x7d810a29cd8a] */
void	print_stackframe(char *frame)
{
	while (*frame)
	{
		if (*frame == '(')
			(ft_putchar(*frame), ft_putstr("\033[41;30m"));
		else if (*frame == '+' || *frame == ')')
			(ft_putstr("\033[m"), ft_putchar(*frame));
		else
			ft_putchar(*frame);
		++frame;
	}
	ft_putchar('\n');
}

/* print current callstack, minus this function itself and minus 3 top-levels,
 * which are usually libc and _start
 */
void	print_callstack(void)
{
	void	*callstack[MAX_CALLSTACK_SIZE];
	int		frames;
	char	**symbols;

	(void)frames;
	(void)symbols;
	(void)callstack;
	ft_printf("If it was allowed, this would now print a stacktrace."
		"But since it is not allowed, you have to use valgrind or gdb.");
}
	/* frames = backtrace(callstack, MAX_CALLSTACK_SIZE); */
	/* symbols = backtrace_symbols(callstack, frames); */
	/* frames -= 3; */
	/* while (--frames) */
		/* print_stackframe(symbols[frames]); */
	/* free(symbols); */
	/* ft_printf("\n"); */
