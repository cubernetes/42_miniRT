/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 20:05:14 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/18 20:05:14 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>
#include <stdio.h>

void	new_cylinder(t_cylinder *this, t_vec3 *center, t_vec3 *axis, double radius, double height)
{
	double	ratio;

	if (radius <= 0)
	{
		ft_dprintf(2, "Error: radius of the cylinder should be positive\n");
		return ;
	}
	if (height <= 0)
	{
		ft_dprintf(2, "Error: height of the cylinder should be positive\n");
		return ;
	}
	this->center = center;
	this->axis = axis;
	this->radius = radius;
	this->height = height;
	ratio = height * height / (4 * length_squared_vec3(axis));
	this->base_top = ft_calloc(sizeof(t_vec3), 1);
	copy_vec3(this->base_top, axis);
	sc_mult_vec3(this->base_top, sqrt(ratio));
	add_vec3(this->base_top, center);
	this->base_bot = ft_calloc(sizeof(t_vec3), 1);
	copy_vec3(this->base_bot, axis);
	sc_mult_vec3(this->base_bot, -sqrt(ratio));
	add_vec3(this->base_bot, center);
	print_cylinder(this);
}

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

void	norm_point_to_line(t_vec3 *norm, t_vec3 *point, t_ray *ray)
{
/*
 p - radius vector of arbitrary point in space
 h - radius vector of norm base
 l0 - radius vector of arbitrary point of the line/ray
 l - direction vector of the line/ray

 vector equation of perpendicularity of PH and a line:
 (h - p)⋅l = 0 (1)

 vector equation of the line:
 h = l0 + t * l (2)

 substitute h from (2) into (1):
 (l0 + t * l - p)⋅l = 0

 substitute v = p - l0:
 (t * l - v)⋅l = 0

 use dot product properties:
 t * || l || ^ 2 - v⋅l = 0

 express t:
 t = v⋅l / || l || ^ 2 (3)

 substitute t from (3) into (2):
 h = l0 + (v⋅l / || l || ^ 2) * l
*/
	t_vec3	v;
	t_vec3	offset;
	double	t;

	copy_vec3(&v, point);
	substract_vec3(&v, ray->terminus);
	t = dot_product_vec3(&v, ray->vec) / length_squared_vec3(ray->vec);
	copy_vec3(&offset, ray->vec);
	sc_mult_vec3(&offset, t);
	copy_vec3(norm, ray->terminus);
	add_vec3(norm, &offset);
}
static void	intersection_cylinder_sides(double *x, t_cylinder *cylinder, t_ray *ray)
{
/*
 get intersections with top/bottom plane and check if the distance to the top/bottom base is not bigger than radius
*/
	t_plane	top;
	t_plane	bot;
	t_vec3	at_top;
	t_vec3	at_bot;

	new_plane(&top, cylinder->base_top, cylinder->axis);
	intersection_plane(x, &top, ray);
	ray_at(ray, *x, &at_top);
	substract_vec3(&at_top, cylinder->base_top);
	if (length_squared_vec3(&at_top) > cylinder->radius * cylinder->radius)
		*x = NO_ROOTS;
	new_plane(&bot, cylinder->base_bot, cylinder->axis);
	intersection_plane(x + 1, &bot, ray);
	ray_at(ray, *(x + 1), &at_bot);
	substract_vec3(&at_bot, cylinder->base_bot);
	if (length_squared_vec3(&at_bot) > cylinder->radius * cylinder->radius)
		*(x + 1) = NO_ROOTS;
}

static void	choose_root(double *t, double *x)
{
	double	min_x;
	int		i = 0;

	min_x = NO_ROOTS;
	i = -1;
	while (++i < 4)
	{
		if ((x[i] >= 0) && (min_x == -1 || x[i] < min_x))
			min_x = x[i];
	}
	*t = min_x;
}

void intersection_cylinder(double *t, t_cylinder *cylinder, t_ray *ray)
{
/*
 v - orientation vector of the axis of the cylinder
 c - center of the cylinder
 r - radius of the cylinder
 p - intersection point
 l0 - terminus of the ray
 l - orientation vector of the ray
 h - base of the norm from p to the axis of cylinder

 triangle law:
 h - p = (h - c) + (c - p)

 CH is a projection of CP on the axis of cylinder, so rewrite using dot product:
 h - p = (p - c)⋅v * v + (c - p)

 Take magnitude of both sides and square it up:
 r ^ 2 = || h - p || ^ 2 = || (p - c)⋅v * v + (c - p) || ^ 2

 substitute k = p - c:
 || k⋅v * v - k || ^ 2 = r ^ 2

 use dot product laws:
 (k⋅v) ^ 2 * || v || ^ 2 + || k || ^ 2 - 2 * (k⋅v) ^ 2 = r ^ 2

 group up:
 || k || ^ 2 + (k⋅v) ^ 2 * (|| v || ^ 2 - 2) - r ^ 2 = 0 (1)

 equation of a ray:
 p = l0 + t * l (2)

 replace k back, substitute q = l0 - c and p from (2) into (1):
 || t * l + q || ^ 2 + ((t * l + q)⋅v) ^ 2 * (|| v || ^ 2 - 2) - r ^ 2 (3)

 convert || t * l + q || ^ 2:
 || t * l + q || ^ 2 = t ^ 2 * || l || ^ 2 + 2 * t * l⋅q + || q || ^ 2 (4)

 convert ((t * l + q)⋅v) ^ 2:
 ((t * l + q)⋅v) ^ 2 = (t * l⋅v + q⋅v) ^ 2 = t ^ 2 * (l⋅v) ^ 2 + 2 * t * (l⋅v) * (q⋅v) + (q⋅v) ^ 2 (5)

 substitute (4) and (5) into (3):
 t ^ 2 * || l || ^ 2 + 2 * t * l⋅q + || q || ^ 2 + ((t ^ 2 * (l⋅v) ^ 2 + 2 * t * (l⋅v) * (q⋅v) + (q⋅v) ^ 2) * (|| v || ^ 2 - 2)) - r ^ 2 = 0

 simplify and transform to a quadratic equation:
 t ^ 2 * (|| l || ^ 2 + (l⋅v) ^ 2 * (|| v || ^ 2 - 2)) + 2 * t * (l⋅q + (l⋅v) * (q⋅v) * (|| v || ^ 2 - 2)) + (|| q || ^ 2 - r ^ 2 + (q⋅v) ^ 2 * (|| v || ^ 2 - 2)) = 0

 retrieve square equation coefficients
 A = || l || ^ 2 + (l⋅v) ^ 2 * (|| v || ^ 2 - 2)
 B = 2 * (l⋅q + (l⋅v) * (q⋅v) * (|| v || ^ 2 - 2))
 C = || q || ^ 2 - r ^ 2 + (q⋅v) ^ 2 * (|| v || ^ 2 - 2)

 substitute d0 = l⋅v, d1 = l⋅q, d2 = q⋅v, d3 = || v || ^ 2 - 2:
 A = || l || ^ 2 + d0 ^ 2 * d3
 B = 2 * (d1 + d0 * d2 * d3)
 C = || q || ^ 2 - r ^ 2 + d2 ^ 2 * d3

 calculate discriminant D:
 D = sqrt(B ^ 2 - 4AC)
 if D is negative, no intersections are possible.

 calculate roots:
 x1 = -(B + D) / 2A
 x2 = -(B - D) / 2A

 check if they are non-negative and intersect with the "real" part of cylinder:
 x1 >= 0 && || h1 - c || <= height
 x2 >= 0 && || h2 - c || <= height

 calculate intersections with top and bottom part of cylinder:

 || base_top - intersection_top || <= r
 || base_bot - intersection_bot || <= r

 choose the lowest valid from x1, x2, intersection_top, intersection_bot
 */
	t_vec3	q;
	double d[4];
	double sec[3];
	double discriminant;
	double x[4];

	copy_vec3(&q, ray->terminus);
	substract_vec3(&q, cylinder->center);
	d[0] = dot_product_vec3(ray->vec, cylinder->axis);
	d[1] = dot_product_vec3(ray->vec, &q);
	d[2] = dot_product_vec3(&q, cylinder->axis);
	d[3] = length_squared_vec3(cylinder->axis) - 2;
	sec[0] = length_squared_vec3(ray->vec) + d[0] * d[0] * d[3];
	sec[1] = 2 * (d[1] + d[0] * d[2] * d[3]);
	sec[2] = length_squared_vec3(&q) - cylinder->radius * cylinder->radius + d[2] * d[2] * d[3];
	discriminant = sec[1] * sec[1] - 4 * sec[0] * sec[2];
	if (discriminant < 0)
	{
		*t = NO_ROOTS;
		return ;
	}
	discriminant = sqrt(discriminant);
	x[0] = (-sec[1] - discriminant) / (2 * sec[0]);
	x[1] = (-sec[1] + discriminant) / (2 * sec[0]);//todo: check if roots are in the "real range"
	intersection_cylinder_sides(x + 2, cylinder, ray);
	choose_root(t, x);
}
