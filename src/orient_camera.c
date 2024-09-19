/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orient_camera.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:10:01 by tosuman           #+#    #+#             */
/*   Updated: 2024/09/19 13:32:56 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

#include <math.h>

/* todo unimportant: either rotate view direction vector and then recalculate
 * the other 2 OR: rotate each vector using 3 different quaternions? */
void	rotate_camera(t_camera *camera, t_direction direction, double degrees)
{
	t_quat	quat;
	t_vec3	left_vec;

	if (camera->dir.x == 0.0 && camera->dir.z == 0.0)
		camera->dir.x += 0.1;
	if (direction == DIR_LEFT)
		new_unit_quat(&quat, degrees, &(t_vec3){.x = 0, .y = 1, .z = 0});
	else if (direction == DIR_RIGHT)
		new_unit_quat(&quat, -degrees, &(t_vec3){.x = 0, .y = 1, .z = 0});
	else if (direction == DIR_UP)
		new_unit_quat(&quat, degrees, &camera->right);
	else if (direction == DIR_DOWN)
		new_unit_quat(&quat, -degrees, &camera->right);
	else if (direction == DIR_FORWARD)
		new_unit_quat(&quat, degrees, &camera->dir);
	else if (direction == DIR_BACKWARD)
		new_unit_quat(&quat, -degrees, &camera->dir);
	rotate_vec3(&camera->dir, &quat);
	copy_vec3(&left_vec, &camera->dir);
	cross_product_vec3(&left_vec, &(t_vec3){.x = 0, .y = -1, .z = 0});
	unit_vec3(&left_vec);
	copy_vec3(&camera->right, &left_vec);
	sc_mult_vec3(&camera->right, -1);
	copy_vec3(&camera->up, &camera->dir);
	cross_product_vec3(&camera->up, &left_vec);
}

void	camera_yaw(t_scene *scene, int amount)
{
	rotate_camera(scene->camera, DIR_LEFT, amount / CAM_ROTATE_FACTOR);
}

void	camera_pitch(t_scene *scene, int amount)
{
	double			dot;

	dot = dot_product_vec3(&scene->camera->dir,
			&(t_vec3){.x = 0, .y = (amount > 0) - (amount < 0), .z = 0});
	if (cos(amount * PI / 180.0 / CAM_ROTATE_FACTOR) > dot)
		rotate_camera(scene->camera, DIR_UP, amount / CAM_ROTATE_FACTOR);
}
