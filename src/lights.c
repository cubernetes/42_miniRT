/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 19:56:03 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/23 19:56:03 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <stdio.h>

void	apply_light(t_color *color, t_color ratio)
{
	set_red(color, (unsigned int)(get_red(color) * get_red(&ratio) / 256));
	set_green(color, (unsigned int)(get_green(color) * get_green(&ratio) / 256));
	set_blue(color, (unsigned int)(get_blue(color) * get_blue(&ratio) / 256));
}

void	combine_light(t_color *color, t_light *light)
{
	const unsigned int	new_red = (unsigned int)(get_red(color) + get_red(&light->color) * light->ratio);
	const unsigned int	new_green = (unsigned int)(get_green(color) + get_green(&light->color) * light->ratio);
	const unsigned int	new_blue = (unsigned int)(get_blue(color) + get_blue(&light->color) * light->ratio);

	if (new_red > 255)
		set_red(color, 255);
	else
		set_red(color, new_red);
	if (new_green > 255)
		set_green(color, 255);
	else
		set_green(color, new_green);
	if (new_blue > 255)
		set_blue(color, 255);
	else
		set_blue(color, new_blue);
}

void print_double_byte_by_byte(double value) {
	// Cast the double to a pointer to a char (byte)
	unsigned char *bytePtr = (unsigned char*)&value;

	// Print each byte in hexadecimal format
	for (int i = 0; (unsigned int)i < sizeof(double); i++) {
		printf("%02X ", bytePtr[i]);
	}
	printf("\n");
}

t_color	calculate_lighting(t_vec3 *point, t_obj *objects, t_scene *scene)
{
	t_ray	ray;
	t_vec3	orientation;
	int		i;
	double	t;
	t_color	res;

	res = 0;
	i = -1;
	while (++i < scene->nb_lights)//0-th lighting that is located in [0, 0, 0] is an ambience lighting
	{
		t = NO_ROOTS;
		copy_vec3(&orientation, scene->lights[i].point);
		substract_vec3(&orientation, point);
		new_ray(&ray, point, &orientation);
		cast_ray(&t, &ray, objects, scene->nb_objs);
		if ((int)t <= 0 || t > 1.0)
		{
			combine_light(&res, &(scene->lights[i]));
		}
//		else
//			print_double_byte_by_byte(t);
	}
	return (res);
}
