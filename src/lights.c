/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 00:38:28 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/19 08:32:26 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

#include <stdio.h>
#include <math.h>

void	apply_light(t_color *color, t_color ratio)
{
	set_red(color, (unsigned int)(get_red(color) * get_red(&ratio) / 256));
	set_green(color, (unsigned int)(get_green(color)
			* get_green(&ratio) / 256));
	set_blue(color, (unsigned int)(get_blue(color) * get_blue(&ratio) / 256));
}

static void	combine_light(t_color *color, t_light *light,
	float diffusion_factor)
{
	const unsigned int	new_red = (unsigned int)((float)get_red(color)
			+ (float)get_red(&light->color) * light->ratio * diffusion_factor);
	const unsigned int	new_green = (unsigned int)((float)get_green(color)
			+ (float)get_green(&light->color) * light->ratio * diffusion_factor);
	const unsigned int	new_blue = (unsigned int)((float)get_blue(color)
			+ (float)get_blue(&light->color) * light->ratio * diffusion_factor);

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

static void	init_shadow_ray(t_hit *shadow_hit, t_hit *hit,
	t_light *light, t_ray *ray)
{
	static t_vec3	orientation;

	shadow_hit->t = NO_ROOTS;
	copy_vec3(&orientation, light->point);
	substract_vec3(&orientation, &hit->point);
	unit_vec3(&orientation);
	new_ray(ray, &hit->point, &orientation);
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

/* expensive function, is run:
 *     window_width * window_height * nb_lights * nb_objs times (roughly 3 mil.)
 */
t_color	calculate_lighting(t_hit *hit, t_scene *scene)
{
	t_ray	ray;
	int		i;
	t_hit	shadow_hit;
	t_color	res;
	t_vec3	orig_vec;

	res = 0;
	combine_light(&res, scene->lights[0], scene->lights[0]->ratio);
	i = 0;
	while (++i < scene->nb_lights)
	{
		copy_vec3(&orig_vec, scene->lights[i]->point);
		substract_vec3(&orig_vec, &hit->point);
		init_shadow_ray(&shadow_hit, hit, scene->lights[i], &ray);
		if (dot_product_vec3(&hit->norm, ray.vec)
			* dot_product_vec3(&hit->norm, &hit->ray_dir) < 0.0
			&& (cast_ray(&shadow_hit, &ray, scene)
			|| shadow_hit.t * shadow_hit.t > length_squared_vec3(&orig_vec)))
		{
			combine_light(&res, scene->lights[i],
					fabsf(cos_vec3(&hit->norm, ray.vec)));
		}
	}
	if (hit->object->selected)
		res = 0x00FF00FF - res;
	return (res);
}
