/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:32:26 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 11:33:33 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	color_to_vec3(t_vec3 *vec, t_color color)
{
	vec->r = get_red(&color);
	vec->g = get_green(&color);
	vec->b = get_blue(&color);
}

void	vec3_to_color(t_vec3 *vec, t_color *color)
{
	set_red(color, (unsigned int)vec->r);
	set_green(color, (unsigned int)vec->g);
	set_blue(color, (unsigned int)vec->b);
}

t_vec3	*get_object_pos(t_obj *obj)
{
	if (obj->type == TOK_SPHERE)
		return (obj->sphere.center);
	else if (obj->type == TOK_PLANE)
		return (obj->plane.point);
	return (obj->cylinder.center);
}

char	*obj_type_to_str(t_obj *obj)
{
	if (obj->type == TOK_SPHERE)
		return ("sphere");
	else if (obj->type == TOK_PLANE)
		return ("plane");
	return ("cylinder");
}
