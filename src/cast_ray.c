/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 02:37:41 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/12 01:14:55 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

#include "float.h"
#include <math.h>

void	calculate_norm_cylinder(t_hit *hit)
{
	t_vec3	norm_base;
	t_vec3	temp;
	t_ray	ray;
	double	norm_len_squared;

	new_ray(&ray, hit->object->cylinder.center, hit->object->cylinder.axis);
	norm_point_to_line(&norm_base, &hit->point, &ray);
	copy_vec3(&hit->norm, &hit->point);
	substract_vec3(&hit->norm, &norm_base);
	norm_len_squared = length_squared_vec3(&hit->norm);
	if (hit->object->cylinder.radius * hit->object->cylinder.radius
		- norm_len_squared > EPSILON2)
	{
		copy_vec3(&temp, &hit->point);
		substract_vec3(&temp, hit->object->cylinder.base_bot);
		copy_vec3(&hit->norm, hit->object->cylinder.axis);
		if (fabs(norm_len_squared - length_squared_vec3(&temp)) > EPSILON2)
			sc_mult_vec3(&hit->norm, -1);
	}
}

void	calculate_norm(t_hit *hit)
{
	if (hit->object->type == TOK_PLANE)
		copy_vec3(&hit->norm, hit->object->plane.norm);
	else if (hit->object->type == TOK_SPHERE)
	{
		copy_vec3(&hit->norm, &hit->point);
		substract_vec3(&hit->norm, hit->object->sphere.center);
	}
	else if (hit->object->type == TOK_CYLINDER)
		calculate_norm_cylinder(hit);
}

void	calculate_hit(t_hit *hit, t_ray *ray, t_obj *object, double *old_t)
{
	if (object->type == TOK_PLANE)
		intersection_plane(&hit->t, &object->plane, ray);
	else if (object->type == TOK_SPHERE)
		intersection_sphere(&hit->t, &object->sphere, ray);
	else if (object->type == TOK_CYLINDER)
		intersection_cylinder(&hit->t, &object->cylinder, ray);
	if ((hit->t > 0) && (hit->t < *old_t || *old_t == NO_ROOTS))
	{
		*old_t = hit->t;
		hit->object = object;
		hit->color = object->color;
	}
}

int	cast_ray(t_hit *hit, t_ray *ray, t_scene *scene)
{
	int		i;
	double	old_t;

	i = -1;
	hit->t = NO_ROOTS;
	old_t = NO_ROOTS;
	hit->color = 0x00000000;
	while (++i < scene->nb_objs)
		calculate_hit(hit, ray, scene->objects[i], &old_t);
	if (old_t == NO_ROOTS)
	{
		hit->t = NO_ROOTS;
		return (EXIT_FAILURE);
	}
	hit->t = old_t;
	(void)ray_at(ray, hit->t, &(hit->point));
	calculate_norm(hit);
	copy_vec3(&hit->ray_dir, ray->vec);
	return (EXIT_SUCCESS);
}
