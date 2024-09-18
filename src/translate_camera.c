/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate_camera.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 21:47:54 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/18 21:45:54 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

// todo: check why it's wrong
static bool	translate_camera_u_d(t_camera *camera,
	t_direction direction, double amount)
{
	if (direction == DIR_UP)
		camera->pos.y += amount;
	else
		camera->pos.y -= amount;
	return (true);
}

static bool	translate_camera_f_b(t_camera *camera,
	t_direction direction, double amount)
{
	t_vec3	direction_vector;

	if (camera->dir.x == 0 && camera->dir.z == 0)
	{
		ft_printf("Looking straight up, cannot move forward/backward\n");
		return (false);
	}
	copy_vec3(&direction_vector, &camera->dir);
	direction_vector.y = 0;
	unit_vec3(&direction_vector);
	sc_mult_vec3(&direction_vector, amount);
	if (direction == DIR_FORWARD)
		add_vec3(&camera->pos, &direction_vector);
	else
		substract_vec3(&camera->pos, &direction_vector);
	return (true);
}

static bool	translate_camera_l_r(t_camera *camera,
	t_direction direction, double amount)
{
	t_vec3			direction_vector;
	static t_vec3	upwards_vector = {.x = 0, .y = 1, .z = 0};

	if (camera->dir.x == 0 && camera->dir.z == 0)
	{
		ft_printf("Looking straight up, cannot move left/right\n");
		return (false);
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
	return (true);
}

bool	translate_camera(t_camera *camera, t_direction direction, double amount)
{
	bool	moved;

	moved = false;
	if (direction == DIR_UP || direction == DIR_DOWN)
		moved |= translate_camera_u_d(camera, direction, amount);
	else if (direction == DIR_FORWARD || direction == DIR_BACKWARD)
		moved |= translate_camera_f_b(camera, direction, amount);
	else
		moved |= translate_camera_l_r(camera, direction, amount);
	return (moved);
}
