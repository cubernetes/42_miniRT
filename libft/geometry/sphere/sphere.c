/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 22:06:44 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/07 22:06:44 by nam-vu           ###   ########.fr       */
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
int	intersection_sphere(double *t, t_sphere *sphere, t_ray *ray)
{
	double	discriminant;
	double	a[3];
	t_vec3	v;

	copy_vec3(&v, ray->terminus);
	substract_vec3(&v, sphere->center);
	a[0] = length_squared_vec3(ray->vec);
	a[1] = 2 * dot_product_vec3(ray->vec, &v);
	a[2] = length_squared_vec3(&v) - sphere->radius * sphere->radius;
	discriminant = a[1] * a[1] - 4 * a[0] * a[2];
	if (discriminant >= 0)
	{
		discriminant = sqrt(discriminant);
		*t = -a[1] - discriminant;
		if (*t <= 0)
			*t = -a[2] + discriminant;
		if (*t > 0)
		{
			*t /= 2 * a[0];
			return (1);
		}
	}
	*t = NO_ROOTS;
	return (0);
}
