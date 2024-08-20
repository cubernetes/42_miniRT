/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 22:52:39 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/20 23:05:00 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>
#include <stdio.h>

void	new_sphere(t_sphere *this, t_vec3 *center, double radius)
{
	this->center = center;
	if (radius <= 0)
	{
		ft_dprintf(2, "Error: radius of the sphere should be positive\n");
		return ;
	}
	this->radius = radius;
}

void	copy_sphere(t_sphere *this, t_sphere *sphere)
{
	copy_vec3(this->center, sphere->center);
	this->radius = sphere->radius;
}

void	print_sphere(t_sphere *this)
{
	printf("Sphere:\n");
	printf("\tsphere radius = %f\n\tsphere center: ", this->radius);
	print_vec3(this->center);
}

/*
 o - radius-vector of the center of the sphere
 p - radius-vector of the point on the sphere
 r - radius of the sphere
 l0 - radius vector of the terminus of the ray
 l - direction vector of the ray

 vector equation of the sphere:
 ||p - o|| = r (1)

 vector equation of the ray:
 p = l0 + t * l (2)

 substitute p in (1) with (2) and square up both sides:
 || l0 + t * l - o || ^ 2 = r ^ 2

 substitute v = l0 - o:
 || t * l + v || ^ 2 = r ^ 2

 use dot product properties:
 (t ^ 2 * || l || ^ 2) + (2t * l⋅v) + (|| v || ^ 2 - r ^ 2) = 0

 retrieve square equation coefficients
 A = || l || ^ 2
 B = 2 l⋅v
 C = || v || ^ 2 - r ^ 2

 calculate discriminant D:
 D = sqrt(B ^ 2 - 4AC)
 if D is negative, no intersections are possible.

 calculate roots:
 x1 = -(B + D) / 2A
 x2 = -(B - D) / 2A

 take lowest non-negative one:
 x = x1 >= 0 ? x1 : (x2 >= 0 ? x2 : NaN)

*/
void	intersection_sphere(double *t, t_sphere *sphere, t_ray *ray)
{
	double	discriminant;
	double	a;
	double	b;
	double	c;
	t_vec3	v;

	copy_vec3(&v, ray->terminus);
	substract_vec3(&v, sphere->center);
	a = length_squared_vec3(ray->vec);
	b = 2 * dot_product_vec3(ray->vec, &v);
	c = length_squared_vec3(&v) - sphere->radius * sphere->radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
	{
		*t = NO_ROOTS;
		return ;
	}
	discriminant = sqrt(discriminant);
	*t = -b - discriminant;
	if (*t >= 0)
	{
		*t /= 2 * a;
		return ;
	}
	*t = -b + discriminant;
	if (*t >= 0)
	{
		*t /= 2 * a;
		return ;
	}
	*t = NO_ROOTS;
}
