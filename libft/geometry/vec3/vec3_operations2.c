/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_operations2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 22:32:48 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/16 22:32:48 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	add_vec3(t_vec3 *this, t_vec3 *vec)
{
	this->e[0] += vec->e[0];
	this->e[1] += vec->e[1];
	this->e[2] += vec->e[2];
}

void	substract_vec3(t_vec3 *this, t_vec3 *vec)
{
	this->e[0] -= vec->e[0];
	this->e[1] -= vec->e[1];
	this->e[2] -= vec->e[2];
}

double	dot_product_vec3(t_vec3 *this, t_vec3 *vec)
{
	return (this->e[0] * vec->e[0]
		+ this->e[1] * vec->e[1]
		+ this->e[2] * vec->e[2]);
}

void	cross_product_vec3(t_vec3 *this, t_vec3 *vec)
{
	new_vec3(this, this->e[1] * vec->e[2] - this->e[2] * vec->e[1],
		this->e[2] * vec->e[0] - this->e[0] * vec->e[2],
		this->e[0] * vec->e[1] - this->e[1] * vec->e[0]);
}

void	unit_vec3(t_vec3 *this)
{
	div_vec3(this, length_vec3(this));
}
