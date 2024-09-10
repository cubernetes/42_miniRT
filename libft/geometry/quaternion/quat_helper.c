/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 21:52:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/07 22:48:21 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// todo: potential speedup possible
// 1 == sqrt(cos(degrees * PI / 360)**2 + length_squared_vec3(x, y, z));
// 1 == cos(degrees * PI / 360)**2 + length_squared_vec3(x, y, z);
// 1 - cos(degrees * PI / 360)**2 == length_squared_vec3(x_new, y_new, z_new);
// curr_length == length_squared_vec3(x_curr, y_curr, z_curr);
// x_new / x_curr = sin(degrees * PI / 360) / sqrt(curr_length);

//todo: maybe use cosf
// expects a unit vector!!!
// scales the axis!!! don't use it afterwards
// WATCH OUT, the following line might be missing before (or after?) sc_mult_vec
/* unit_vec3(&unit_axis); */
void	new_unit_quat(t_quat *quat, double degrees, t_vec3 *axis)
{
	t_vec3	unit_axis;

	copy_vec3(&unit_axis, axis);
	sc_mult_vec3(&unit_axis, sin(degrees * PI / 360));
	quat_copy(quat, &(t_quat){
		.scalar = cos(degrees * PI / 360),
		.x = unit_axis.x,
		.y = unit_axis.y,
		.z = unit_axis.z
	});
}
