#include "libft.h"

#include <stdio.h>
#include <math.h>

void	quat_conj(t_quat *quat)
{
	reverse_vec3(&quat->vector);
}

double	quat_len_squared(t_quat *quat)
{
	return (quat->scalar * quat->scalar + length_squared_vec3(&quat->vector));
}

double	quat_len(t_quat *quat)
{
	return (sqrt(quat_len_squared(quat)));
}

void	quat_print(t_quat *quat)
{
	printf("Quaternion: [%f, %f, %f, %f]\n",
		quat->scalar, quat->i, quat->j, quat->k);
}
