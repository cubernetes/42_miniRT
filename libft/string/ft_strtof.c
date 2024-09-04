/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtof.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 09:26:36 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/04 15:51:09 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <math.h>
#include "float.h"

static double	get_real_part(long double *number, const char **s)
{
	while (ft_isdigit(**s))
	{
		*number = 10.0 * *number + (**s - '0');
		(*s)++;
	}
}

double	ft_strtof(const char *s)
{
	int			sign;
	long double	number;
	long double	power;

	number = 0.0;
	power = 1.0;
	while (ft_isspace(*s))
		++s;
	sign = (('-' != *s) - 1) * 2 + 1;
	if ('-' == *s || '+' == *s)
		++s;
	if (!ft_isdigit(*s))
		return (NAN);
	get_real_part(&number, &s);
	if (*s == '.')
		++s;
	while (ft_isdigit(*s))
	{
		number = 10.0 * number + (*s - '0');
		power *= 10.0;
		++s;
	}
	if (*s != 0)
		return (NAN);
	return (sign * number / power);
}
