#include "libft.h"

#include <stdio.h>
#include <stdlib.h>

// todo: add quaternions
// todo: add transformations

void	init(void)
{
	(void)set_allocator(gc_malloc);
	(void)gc_set_context("DEFAULT");
}

void	finish(int exit_status)
{
	gc_free_all();
	exit(exit_status);
}

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
	t_ray		*ray;
	t_vec3		intersection;

	init();
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
	ray = &(t_ray){.terminus = terminus, .vec = ray_vec};
	t = NO_ROOTS;
	if (intersection_cylinder(&t, &cylinder, ray))
		printf("No intersection are possible\n");
	else
	{
		printf("t = %f, intersection point: ", t);
		(void)ray_at(ray, t, &intersection);
		print_vec3(&intersection);
	}
	finish(0);
}
