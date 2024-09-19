/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 22:36:46 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/19 07:56:03 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <math.h>
#include <stdio.h>

void	copy_cylinder(t_cylinder *this, t_cylinder *cylinder)
{
	copy_vec3(this->center, cylinder->center);
	copy_vec3(this->axis, cylinder->axis);
	this->radius = cylinder->radius;
	this->height = cylinder->height;
	this->base_top = cylinder->base_top;
	this->base_bot = cylinder->base_bot;
}

void	print_cylinder(t_cylinder *this)
{
	printf("Cylinder:\n");
	printf("\tCylinder radius = %f\n\tCylinder"
		" height = %f\n\tCylinder center: ", this->radius, this->height);
	print_vec3(this->center);
	printf("\tCylinder axis: ");
	print_vec3(this->axis);
	printf("\tCylinder base top: ");
	print_vec3(this->base_top);
	printf("\tCylinder base bot: ");
	print_vec3(this->base_bot);
}

void	choose_root(float *t, float *x)
{
	float	min_x;
	int		i;

	min_x = NO_ROOTS;
	i = -1;
	while (++i < 4)
	{
		if ((x[i] > EPSILON_ROOT) && (min_x == -1 || x[i] < min_x))
			min_x = x[i];
	}
	*t = min_x;
}

void	calculate_products_cylinder(float *d, t_cylinder *cylinder,
	t_ray *ray, t_vec3 *q)
{
	copy_vec3(q, ray->terminus);
	substract_vec3(q, cylinder->center);
	d[0] = dot_product_vec3(ray->vec, cylinder->axis);
	d[1] = dot_product_vec3(ray->vec, q);
	d[2] = dot_product_vec3(q, cylinder->axis);
}

void	calculate_sec_cylinder(float *sec, t_ray *ray, float *d)
{
	sec[0] = length_squared_vec3(ray->vec) - d[0] * d[0];
	sec[1] = 2 * (d[1] - d[0] * d[2]);
}
