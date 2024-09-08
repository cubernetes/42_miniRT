/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 00:38:28 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/08 00:38:28 by nam-vu           ###   ########.fr       */
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
	double diffusion_factor)
{
	const unsigned int	new_red = (unsigned int)(get_red(color)
			+ get_red(&light->color) * light->ratio * diffusion_factor);
	const unsigned int	new_green = (unsigned int)(get_green(color)
			+ get_green(&light->color) * light->ratio * diffusion_factor);
	const unsigned int	new_blue = (unsigned int)(get_blue(color)
			+ get_blue(&light->color) * light->ratio * diffusion_factor);

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
	copy_vec3(&orientation, &hit->point);
	substract_vec3(&orientation, light->point);
	unit_vec3(&orientation);
	new_ray(ray, light->point, &orientation);
}

static t_color	is_subnormal(t_hit *shadow_hit, t_hit *hit,
	t_light *light, t_ray *ray)
{
	t_vec3	first;

	copy_vec3(&first, &(shadow_hit->point));
	substract_vec3(&first, &hit->point);
	if (length_squared_vec3(&first) <= EPSILON2)
		combine_light(&shadow_hit->color, light,
			fabs(cos_vec3(&hit->norm, ray->vec)));
	return (shadow_hit->color);
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

	res = 0;
	combine_light(&res, scene->lights[0], scene->lights[0]->ratio);
	i = 0;
	while (++i < scene->nb_lights)
	{
		init_shadow_ray(&shadow_hit, hit, scene->lights[i], &ray);
		if (fabs(dot_product_vec3(&hit->norm, ray.vec)
				* dot_product_vec3(&hit->norm, &hit->ray_dir)) > EPSILON)
		{
			if (cast_ray(&shadow_hit, &ray, scene))
				combine_light(&res, scene->lights[i],
					fabs(cos_vec3(&hit->norm, ray.vec)));
			else
			{
				shadow_hit.color = res;
				res = is_subnormal(&shadow_hit, hit, scene->lights[i], &ray);
			}
		}
	}
	return (res);
}
