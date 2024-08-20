/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_operations2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 22:32:48 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/20 23:18:13 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	add_vec3(t_vec3 *this, t_vec3 *vec)
{
	this->x += vec->x;
	this->y += vec->y;
	this->z += vec->z;
}

void	substract_vec3(t_vec3 *this, t_vec3 *vec)
{
	this->x -= vec->x;
	this->y -= vec->y;
	this->z -= vec->z;
}

double	dot_product_vec3(t_vec3 *this, t_vec3 *vec)
{
	return (this->x * vec->x
		+ this->y * vec->y
		+ this->z * vec->z);
}

void	cross_product_vec3(t_vec3 *this, t_vec3 *vec)
{
	new_vec3(this, this->y * vec->z - this->z * vec->y,
		this->z * vec->x - this->x * vec->z,
		this->x * vec->y - this->y * vec->x);
}

void	unit_vec3(t_vec3 *this)
{
	div_vec3(this, length_vec3(this));
}
