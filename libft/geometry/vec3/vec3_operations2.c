/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_operations2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:19:27 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/19 01:00:04 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* TODO unimportant: maybe there's some bitmagic to do this faster? */
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
