#include "miniRT.h"
#include "libft.h"

#include <math.h>
#include <stdio.h>

void assert_norm(t_hit *hit, int index)
{
	if ((fabs(hit->point.x + 1.537377) <= EPSILON) && (fabs(hit->point.y - 17.103315) <= EPSILON) &&
		(fabs(hit->point.z + 30.747532) <= EPSILON))
	{

		if (hit->object->type != TOK_SPHERE)
			exit(printf("NOT A SPHERE\n"));
		if (fabs(length_squared_vec3(&hit->norm) - hit->object->sphere.radius * hit->object->sphere.radius) > EPSILON)
		{
			printf("NORM IS WRONG ");
			if (index == 1)
				printf("IN CAST_RAY ");
			else
				printf("IN RENDER ");
			print_vec3(&hit->norm);
			printf("LENGTH SQUARED = %f\n", length_squared_vec3(&hit->norm));
			printf("RADIUS SQUARED = %f\n", hit->object->sphere.radius * hit->object->sphere.radius);
			printf("ASSERT %i\n", index);
		}
	}
}
