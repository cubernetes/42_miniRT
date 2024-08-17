#include "libft.h"
#include <stdio.h>

int	main(void)
{
	double t;
	t_vec3 center;
	double radius;
	t_sphere sphere;
//	t_vec3 point;
//	t_vec3 norm;
//	t_plane plane;
	t_vec3 terminus;
	t_vec3 ray_vec;
	t_ray ray;

	new_vec3(&center, 0, 0, 0);
	radius = 10;
	new_sphere(&sphere, &center, radius);
//	new_vec3(&point, 1, 1, 1);
//	new_vec3(&norm, 1, 1, 1);
//	new_plane(&plane, &point, &norm);
	new_vec3(&terminus, 0, 0, 0);
	new_vec3(&ray_vec, 1, 2, 3);
	new_ray(&ray, &terminus, &ray_vec);
	t = NO_ROOTS;
	intersection_sphere(&t, &sphere, &ray);
//	intersection_plane(&t, &plane, &ray);
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
