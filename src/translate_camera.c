/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate_camera.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 21:47:54 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/18 06:29:54 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

// todo: check why it's wrong
static void	translate_camera_u_d(t_camera *camera,
	t_direction direction, double amount)
{
	if (direction == DIR_UP)
		camera->pos.y += amount;
	else
		camera->pos.y -= amount;
}

static void	translate_camera_f_b(t_camera *camera,
	t_direction direction, double amount)
{
	t_vec3	direction_vector;

	if (camera->dir.x == 0 && camera->dir.z == 0)
	{
		ft_printf("Looking straight up, cannot move forward/backward\n");
		return ;
	}
	copy_vec3(&direction_vector, &camera->dir);
	direction_vector.y = 0;
	unit_vec3(&direction_vector);
	sc_mult_vec3(&direction_vector, amount);
	if (direction == DIR_FORWARD)
		add_vec3(&camera->pos, &direction_vector);
	else
		substract_vec3(&camera->pos, &direction_vector);
}

static void	translate_camera_l_r(t_camera *camera,
	t_direction direction, double amount)
{
	t_vec3			direction_vector;
	static t_vec3	upwards_vector = {.x = 0, .y = 1, .z = 0};

	if (camera->dir.x == 0 && camera->dir.z == 0)
	{
		ft_printf("Looking straight up, cannot move left/right\n");
		return ;
	}
	copy_vec3(&direction_vector, &camera->dir);
	direction_vector.y = 0;
	unit_vec3(&direction_vector);
	cross_product_vec3(&direction_vector, &upwards_vector);
	sc_mult_vec3(&direction_vector, amount);
	if (direction == DIR_LEFT)
		substract_vec3(&camera->pos, &direction_vector);
	else
		add_vec3(&camera->pos, &direction_vector);
}

void	translate_camera(t_camera *camera, t_direction direction, double amount)
{
	if (direction == DIR_UP || direction == DIR_DOWN)
		translate_camera_u_d(camera, direction, amount);
	else if (direction == DIR_FORWARD || direction == DIR_BACKWARD)
		translate_camera_f_b(camera, direction, amount);
	else
		translate_camera_l_r(camera, direction, amount);
}
