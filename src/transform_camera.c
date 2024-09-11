/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_camera.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 21:47:54 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/12 01:13:41 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

#include <math.h>

// todo: check why it's wrong
static void	translate_camera_u_d(t_scene *scene,
	t_direction direction, double amount)
{
	if (direction == DIR_UP)
		scene->camera->pos.y += amount;
	else
		scene->camera->pos.y -= amount;
}

static void	translate_camera_f_b(t_scene *scene,
	t_direction direction, double amount)
{
	t_vec3	direction_vector;

	if (scene->camera->dir.x == 0 && scene->camera->dir.z == 0)
	{
		ft_printf("Looking straight up, cannot move forward/backward\n");
		return ;
	}
	copy_vec3(&direction_vector, &scene->camera->dir);
	direction_vector.y = 0;
	unit_vec3(&direction_vector);
	sc_mult_vec3(&direction_vector, amount);
	if (direction == DIR_FORWARD)
		add_vec3(&scene->camera->pos, &direction_vector);
	else
		substract_vec3(&scene->camera->pos, &direction_vector);
}

static void	translate_camera_l_r(t_scene *scene,
	t_direction direction, double amount)
{
	t_vec3			direction_vector;
	static t_vec3	upwards_vector = {.x = 0, .y = 1, .z = 0};

	if (scene->camera->dir.x == 0 && scene->camera->dir.z == 0)
	{
		ft_printf("Looking straight up, cannot move left/right\n");
		return ;
	}
	copy_vec3(&direction_vector, &scene->camera->dir);
	direction_vector.y = 0;
	unit_vec3(&direction_vector);
	cross_product_vec3(&direction_vector, &upwards_vector);
	sc_mult_vec3(&direction_vector, amount);
	if (direction == DIR_LEFT)
		substract_vec3(&scene->camera->pos, &direction_vector);
	else
		add_vec3(&scene->camera->pos, &direction_vector);
}

void	translate_camera(t_scene *scene, t_direction direction, double amount)
{
	if (direction == DIR_UP || direction == DIR_DOWN)
		translate_camera_u_d(scene, direction, amount);
	else if (direction == DIR_FORWARD || direction == DIR_BACKWARD)
		translate_camera_f_b(scene, direction, amount);
	else
		translate_camera_l_r(scene, direction, amount);
}

/* todo: either rotate view direction vector and then recalculate the other 2
 * OR: rotate each vector using 3 different quaternions? */
void	rotate_camera(t_camera *camera, t_direction direction, double degrees)
{
	t_quat	quat;
	t_vec3	left_vec;

	(void)direction;
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
