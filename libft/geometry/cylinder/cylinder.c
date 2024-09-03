#include "libft.h"

#include <math.h>
#include <stdio.h>

void	new_cylinder(t_cylinder *this, t_cylinder *cylinder_params)
{
	double	ratio;

	if (cylinder_params->radius <= 0)
	{
		ft_dprintf(2, "Error: radius of the cylinder should be positive\n");
		return ;
	}
	if (cylinder_params->height <= 0)
	{
		ft_dprintf(2, "Error: height of the cylinder should be positive\n");
		return ;
	}
	copy_vec3(this->center, cylinder_params->center);
	copy_vec3(this->axis, cylinder_params->axis);
	this->radius = cylinder_params->radius;
	this->height = cylinder_params->height;
	ratio = cylinder_params->height
		/ (2 * length_vec3(cylinder_params->axis));
	sc_mult_vec3(this->axis, ratio);
	this->base_top = ft_calloc(sizeof(t_vec3), 1);
	copy_vec3(this->base_top, cylinder_params->center);
	add_vec3(this->base_top, cylinder_params->axis);
	this->base_bot = ft_calloc(sizeof(t_vec3), 1);
	copy_vec3(this->base_bot, cylinder_params->center);
	substract_vec3(this->base_bot, cylinder_params->axis);
	unit_vec3(this->axis); // why is this needed?
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
void	norm_point_to_line(t_vec3 *norm, t_vec3 *point, t_ray *ray)
{
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

/*
 get intersections with top/bottom plane and check if the
 distance to the top/bottom base is not bigger than radius
*/
static
void	intersection_cylinder_sides(double *x, t_cylinder *cylinder, t_ray *ray)
{
	t_plane	top;
	t_plane	bot;
	t_vec3	at_top;
	t_vec3	at_bot;

	new_plane(&top, cylinder->base_top, cylinder->axis);
	if (!intersection_plane(x, &top, ray))
	{
		(void)ray_at(ray, *x, &at_top);
		substract_vec3(&at_top, cylinder->base_top);
		if (length_squared_vec3(&at_top) - cylinder->radius * cylinder->radius > 0.0001)
			*x = NO_ROOTS;
	}
	else
		*x = NO_ROOTS;
	new_plane(&bot, cylinder->base_bot, cylinder->axis);
	if (!intersection_plane(x + 1, &bot, ray))
	{
		(void)ray_at(ray, *(x + 1), &at_bot);
		substract_vec3(&at_bot, cylinder->base_bot);
		if (length_squared_vec3(&at_bot) - cylinder->radius * cylinder->radius > 0.0001)
			*(x + 1) = NO_ROOTS;
	}
	else
		*(x + 1) = NO_ROOTS;
}

static void	choose_root(double *t, double *x)
{
	double	min_x;
	int		i;

	min_x = NO_ROOTS;
	i = -1;
	while (++i < 4)
	{
		if ((x[i] > 0) && (min_x == -1 || x[i] < min_x))
			min_x = x[i];
	}
	*t = min_x;
}

/* can be put in choose_root */
static void	is_real_cylinder(double *x, t_cylinder *cylinder, t_ray *ray)
{
	t_vec3	root1;
	t_vec3	root2;
	double	max_distance_squared;

	max_distance_squared = cylinder->radius
		* cylinder->radius + cylinder->height * cylinder->height / 4;
	if (!ray_at(ray, *x, &root1))
	{
		substract_vec3(&root1, cylinder->center);
		if (length_squared_vec3(&root1) > max_distance_squared)
			*x = NO_ROOTS;
	}
	else
		*x = NO_ROOTS;
	if (!ray_at(ray, *(x + 1), &root2))
	{
		substract_vec3(&root2, cylinder->center);
		if (length_squared_vec3(&root2) > max_distance_squared)
			*(x + 1) = NO_ROOTS;
	}
	else
		*(x + 1) = NO_ROOTS;
}

/*
 v - normalized orientation vector of the axis of the cylinder
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
 ((t * l + q)⋅v) ^ 2 = (t * l⋅v + q⋅v) ^ 2
     = t ^ 2 * (l⋅v) ^ 2 + 2 * t * (l⋅v) * (q⋅v) + (q⋅v) ^ 2 (5)

 substitute (4) and (5) into (3):
 t ^ 2 * || l || ^ 2 + 2 * t * l⋅q + || q || ^ 2 + ((t ^ 2 * (l⋅v) ^ 2
     + 2 * t * (l⋅v) * (q⋅v) + (q⋅v) ^ 2) * (|| v || ^ 2 - 2)) - r ^ 2 = 0

 simplify and transform to a quadratic equation:
 t ^ 2 * (|| l || ^ 2 + (l⋅v) ^ 2 * (|| v || ^ 2 - 2))
     + 2 * t * (l⋅q + (l⋅v) * (q⋅v) * (|| v || ^ 2 - 2))
	     + (|| q || ^ 2 - r ^ 2 + (q⋅v) ^ 2 * (|| v || ^ 2 - 2)) = 0

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
int	intersection_cylinder(double *t, t_cylinder *cylinder, t_ray *ray)
{
	t_vec3	q;
	double	d[4];
	double	sec[3];
	double	discriminant;
	double	x[4];

	ft_memmove(x, &(double []){-1, -1, -1, -1}, sizeof(double) * 4);
	copy_vec3(&q, ray->terminus);
	substract_vec3(&q, cylinder->center);
	d[0] = dot_product_vec3(ray->vec, cylinder->axis);
	d[1] = dot_product_vec3(ray->vec, &q);
	d[2] = dot_product_vec3(&q, cylinder->axis);
	d[3] = length_squared_vec3(cylinder->axis) - 2;//todo: replace with -1 since v is a normalized vector
	sec[0] = length_squared_vec3(ray->vec) + d[0] * d[0] * d[3];
	sec[1] = 2 * (d[1] + d[0] * d[2] * d[3]);
	sec[2] = length_squared_vec3(&q)
		- cylinder->radius * cylinder->radius + d[2] * d[2] * d[3];
	discriminant = sec[1] * sec[1] - 4 * sec[0] * sec[2];
	if (discriminant < 0)
		*t = NO_ROOTS;
	else
	{
		discriminant = sqrt(discriminant);
		x[0] = (-sec[1] - discriminant) / (2 * sec[0]);
		x[1] = (-sec[1] + discriminant) / (2 * sec[0]);
		is_real_cylinder(x, cylinder, ray);
	}
	intersection_cylinder_sides(x + 2, cylinder, ray);
	choose_root(t, x);
	return (*t == NO_ROOTS);
}
