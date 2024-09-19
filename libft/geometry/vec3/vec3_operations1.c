/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_operations1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 21:52:38 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/19 08:28:41 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>

void	reverse_vec3(t_vec3 *this)
{
	this->x *= -1;
	this->y *= -1;
	this->z *= -1;
}

void	sc_mult_vec3(t_vec3 *this, float k)
{
	this->x *= k;
	this->y *= k;
	this->z *= k;
}

void	div_vec3(t_vec3 *this, float k)
{
	if (k == 0)
	{
		ft_dprintf(2, "division of vec3 by 0\n");
		return ;
	}
	this->x /= k;
	this->y /= k;
	this->z /= k;
}

// todo later: check all sqrtf calls and mb replace with faster version
float	length_vec3(t_vec3 *this)
{
	return (sqrtf(this->x * this->x
			+ this->y * this->y
			+ this->z * this->z));
}

float	length_squared_vec3(t_vec3 *this)
{
	return (this->x * this->x
		+ this->y * this->y
		+ this->z * this->z);
}
