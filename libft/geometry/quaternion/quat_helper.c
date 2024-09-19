/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 21:52:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 08:29:09 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdio.h>
#include <math.h>

void	quat_conj(t_quat *quat)
{
	reverse_vec3(&quat->vector);
}

float	quat_len_squared(t_quat *quat)
{
	return (quat->scalar * quat->scalar + length_squared_vec3(&quat->vector));
}

float	quat_len(t_quat *quat)
{
	return (sqrtf(quat_len_squared(quat)));
}

void	quat_print(t_quat *quat)
{
	printf("Quaternion: [%f, %f, %f, %f]\n",
		quat->scalar, quat->i, quat->j, quat->k);
}

// potential speedup possible
// 1 == sqrtf(cos(degrees * PI / 360)**2 + length_squared_vec3(x, y, z));
// 1 == cos(degrees * PI / 360)**2 + length_squared_vec3(x, y, z);
// 1 - cos(degrees * PI / 360)**2 == length_squared_vec3(x_new, y_new, z_new);
// curr_length == length_squared_vec3(x_curr, y_curr, z_curr);
// x_new / x_curr = sin(degrees * PI / 360) / sqrtf(curr_length);

//todo later: maybe use cosf
// expects a unit vector!!!
// WATCH OUT, the following line might be missing before (or after?) sc_mult_vec
/* unit_vec3(&unit_axis); */
void	new_unit_quat(t_quat *quat, float degrees, t_vec3 *axis)
{
	t_vec3	unit_axis;

	copy_vec3(&unit_axis, axis);
	sc_mult_vec3(&unit_axis, sinf(degrees * PI / 360));
	quat_copy(quat, &(t_quat){
		.scalar = cosf(degrees * PI / 360),
		.x = unit_axis.x,
		.y = unit_axis.y,
		.z = unit_axis.z
	});
}
