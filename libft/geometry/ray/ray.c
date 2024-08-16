/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 20:28:23 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/16 20:28:23 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	new_ray(t_ray *this, t_point *terminus, t_vec3 *vec)
{
	this->terminus = terminus;
	this->vec = vec;
}

void	copy_ray(t_ray *this, t_ray *ray)
{
	copy_vec3(this->vec, ray->vec);
	copy_point(this->terminus, ray->terminus);
}

void	print_ray(t_ray *this)
{
	ft_printf("Ray:\n");
	ft_printf("\tTerminus: ");
	print_point(this->terminus);
	ft_printf("\tDirection vector: ");
	print_vec3(this->vec);
}
