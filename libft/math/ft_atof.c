/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 03:30:46 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/03 03:30:46 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>
#include "float.h"

double	ft_atof(const char *nptr)
{
	long double	value;
	long double	value_after_point;
	int			sign;
	int			precision;

	value = 0.0;
	value_after_point = 0.0;
	precision = 0;
	sign = 1 * (*nptr != '-' && *nptr != '+') + (-1) * (*nptr == '-')
		+ 0 * ((*nptr == '-' || *nptr == '+') && *(nptr++));
	while (ft_isdigit(*nptr))
	{
		if (value > DBL_MAX || value < DBL_MIN)
			return (INFINITY);
		value = value * 10 + *nptr - 48;
	}
	nptr += (*nptr == '.');
	while (ft_isdigit(*nptr) && ++precision)
		value_after_point = value_after_point * 10 + *nptr - 48;
	if (*nptr != 0)
		return (INFINITY);
	while (--precision >= 0)
		value_after_point /= 100;
	return (sign * (value + value_after_point));
}
