#include "libft.h"
#include <stdio.h>

//todo: add quaternions
//todo: add transformations

int	main(void)
{
	double t;
	t_vec3 center;
	t_vec3 axis;
	double radius;
	double height;
	t_cylinder cylinder;
	t_vec3 terminus;
	t_vec3 ray_vec;
	t_ray ray;

	new_vec3(&axis, 0, 1, 0);
	new_vec3(&center, 0, 11, 0);
	radius = 1;
	height = 1;
	new_cylinder(&cylinder, &center, &axis, radius, height);
	new_vec3(&terminus, 0, 0, 0);
	new_vec3(&ray_vec, 1, 0, 0);
	new_ray(&ray, &terminus, &ray_vec);
	t = NO_ROOTS;
	intersection_cylinder(&t, &cylinder, &ray);
	if (t < 0)
		printf("No intersection are possible\n");
	else
	{
		printf("t = %f, intersection point: ", t);
		t_vec3 intersection;
		ray_at(&ray, t, &intersection);
		print_vec3(&intersection);
	}
}
