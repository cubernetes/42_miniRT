/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_operations1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 22:32:46 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/16 22:32:46 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>

void	reverse_vec3(t_vec3 *this)
{
	this->e[0] *= -1;
	this->e[1] *= -1;
	this->e[2] *= -1;
}

void	sc_mult_vec3(t_vec3 *this, double k)
{
	this->e[0] *= k;
	this->e[1] *= k;
	this->e[2] *= k;
}

void	div_vec3(t_vec3 *this, double k)
{
	if (k == 0)
	{
		ft_dprintf(2, "division of vec3 by 0\n");
		return ;
	}
	this->e[0] /= k;
	this->e[1] /= k;
	this->e[2] /= k;
}

double	length_vec3(t_vec3 *this)
{
	return (sqrt(this->e[0] * this->e[0]
			+ this->e[1] * this->e[1]
			+ this->e[2] * this->e[2]));
}

double	length_squared_vec3(t_vec3 *this)
{
	return (this->e[0] * this->e[0]
		+ this->e[1] * this->e[1]
		+ this->e[2] * this->e[2]);
}
