#include "libft.h"
#include <stdio.h>

int	main(void)
{
	double t;
	t_vec3 center;
	double radius;
	t_vec3 terminus;
	t_vec3 ray_vec;
	t_ray ray;
	t_sphere sphere;

	new_vec3(&center, 5, 5, 5);
	new_vec3(&terminus, 0, 0, 0);
	new_vec3(&ray_vec, 1, 0, 0);
	radius = 4;
	t = NO_ROOTS;
	new_ray(&ray, &terminus, &ray_vec);
	new_sphere(&sphere, &center, radius);
	intersection_sphere(&t, &sphere, &ray);
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
