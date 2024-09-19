/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate_object.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:05:17 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 11:05:54 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

bool	translate_vec3(t_vec3 *vec, t_direction direction, double amount)
{
	if (direction == DIR_FORWARD)
		vec->z -= amount;
	else if (direction == DIR_BACKWARD)
		vec->z += amount;
	else if (direction == DIR_RIGHT)
		vec->x += amount;
	else if (direction == DIR_LEFT)
		vec->x -= amount;
	else if (direction == DIR_UP)
		vec->y += amount;
	else if (direction == DIR_DOWN)
		vec->y -= amount;
	else
	{
		ft_printf("Direction does not exist\n");
		return (false);
	}
	return (true);
}

bool	translate_object(t_obj *obj, t_direction direction, double amount)
{
	bool	moved;

	moved = false;
	if (obj->type == TOK_SPHERE)
		moved |= translate_vec3(obj->sphere.center, direction, amount);
	else if (obj->type == TOK_PLANE)
		moved |= translate_vec3(obj->plane.point, direction, amount);
	else if (obj->type == TOK_CYLINDER)
	{
		moved |= translate_vec3(obj->cylinder.center, direction, amount);
		moved |= translate_vec3(obj->cylinder.base_top, direction, amount);
		moved |= translate_vec3(obj->cylinder.base_bot, direction, amount);
	}
	else
		ft_printf("Object does not exist\n");
	return (moved);
}
