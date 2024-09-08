/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate_camera.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 03:02:11 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/08 03:02:11 by tischmid         ###   ########.fr       */
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
		scene->camera->pos.y -= amount;
	else
		scene->camera->pos.y += amount;
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
