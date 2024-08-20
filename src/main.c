#include "libft.h"
#include <stdio.h>

// todo: add quaternions
// todo: add transformations

int	main(void)
{
	double		t;
	t_vec3		*center;
	t_vec3		*axis;
	double		radius;
	double		height;
	t_cylinder	cylinder;
	t_vec3		*terminus;
	t_vec3		*ray_vec;
	t_ray		ray;
	t_vec3		intersection;

	axis = &(t_vec3){.x = 0, .y = 1, .z = 0};
	center = &(t_vec3){.x = 0, .y = 11, .z = 0};
	radius = 1;
	height = 1;
	new_cylinder(&cylinder, &(t_cylinder){
		.center = center,
		.axis = axis,
		.radius = radius,
		.height = height});
	terminus = &(t_vec3){.x = 0, .y = 0, .z = 0};
	ray_vec = &(t_vec3){.x = 1, .y = 0, .z = 0};
	new_ray(&ray, terminus, ray_vec);
	t = NO_ROOTS;
	intersection_cylinder(&t, &cylinder, &ray);
	if (t < 0)
		printf("No intersection are possible\n");
	else
	{
		printf("t = %f, intersection point: ", t);
		ray_at(&ray, t, &intersection);
		print_vec3(&intersection);
	}
}
