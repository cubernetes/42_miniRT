/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 20:19:55 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/16 20:19:55 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	new_point(t_point *this, double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void	copy_point(t_point *this, t_point *old)
{
	this->x = old->x;
	this->y = old->y;
	this->z = old->z;
}

void	print_point(t_point *this)
{
	ft_printf("Point: [%i, %i, %i]\n", this->x, this->y, this->z);
}
