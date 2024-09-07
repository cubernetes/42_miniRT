/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 22:11:47 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/07 22:11:47 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdio.h>

void	new_plane(t_plane *this, t_vec3 *point, t_vec3 *norm)
{
	this->point = point;
	this->norm = norm;
}

void	copy_plane(t_plane *this, t_plane *plane)
{
	copy_vec3(this->point, plane->point);
	copy_vec3(this->norm, plane->norm);
}

void	print_plane(t_plane *this)
{
	printf("Plane:\n");
	printf("\tpoint on the plane: ");
	print_vec3(this->point);
	printf("\tnorm vector of the plane: ");
	print_vec3(this->norm);
}

int	mod(int a, int b)
{
	const int	r = a % b;

	if (r < 0)
		return (r + b);
	return (r);
}

/*
 a - radius-vector of the point of the plane
 p - radius-vecrot of the point of the ray
 n - norm vector of the plane
 l0 - radius vector of the terminus of the ray
 l - direction vector of the ray

 vector equation of the plane:
 (p - a)⋅n = 0 (1)

 vector equation of the ray:
 p = l0 + t * l (2)

 substitute p in (1) with (2):
 (l0 + t * l - a)⋅n = 0

 substitute v = l0 - a:
 (t * l + v)⋅n = 0

 use dot product properties:
 t * l⋅n + v⋅n = 0

 substitute k = l⋅n, b = v⋅n:
 kt + b = 0

 if k equal 0:
  1) b == 0, then ray is on the plane, take t = 0;
  2) b != 0, then ray is parallel to plane, no intersections are possible;
 otherwise, t = -b/k;
*/
int	intersection_plane(double *t, t_plane *plane, t_ray *ray)
{
	t_vec3	v;
	double	k;
	double	b;

	copy_vec3(&v, ray->terminus);
	substract_vec3(&v, plane->point);
	k = dot_product_vec3(ray->vec, plane->norm);
	b = dot_product_vec3(&v, plane->norm);
	if (k == 0)
	{
		*t = NO_ROOTS;
		return (1);
	}
	*t = -b / k;
	if (*t <= 0)
		*t = NO_ROOTS;
	return (*t == NO_ROOTS);
}
