/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 23:05:03 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/28 03:41:14 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "miniRT.h"
#include "float.h"
#include "mlx.h"

#include <math.h>

#include <stdio.h> // TODO: remove

/* todo: handle no objects edge case */
t_color	cast_ray(double *t, t_ray *ray, t_obj *objects, int nb_objs)
{
	int		i;
	double	old_t;
	t_color	res_color;

	i = -1;
	*t = NO_ROOTS;
	old_t = INFINITY;
	res_color = 0x00000000;
	while (++i < nb_objs)
	{
		if (objects[i].type == PLANE)
			intersection_plane(t, &objects[i].plane, ray);
		else if (objects[i].type == SPHERE)
		{
			intersection_sphere(t, &objects[i].sphere, ray);
			/* if (ray->terminus->x != 0 || ray->terminus->y != 0 || ray->terminus->z != 0) */
			/* { */
				/* if (*t < 0) */
					/* printf("negative:%f\n", *t); */
				/* else */
					/* printf("positive:%f\n", *t); */
			/* } */
		}
		else if (objects[i].type == CYLINDER)
			intersection_cylinder(t, &objects[i].cylinder, ray);
		if ((*t > 0) && (*t < old_t || old_t == NO_ROOTS))
		{
			old_t = *t;
			res_color = objects[i].color;
		}
	}
	if (old_t == INFINITY)
		*t = NO_ROOTS;
	else
		*t = old_t;
	return (res_color);
}

#include <stdio.h>
/* todo: change to FOV */
void	render(t_gc *gc, t_scene *scene, t_obj *objects)
{
	int				x;
	int				y;
	t_color			res_color;
	t_ray			ray;
	t_vec3			terminus;
	t_vec3			orientation;
	t_vec3			intersection;
	double			t;
	const double	scale = 16.0;
	const double	focal_distance = -10.0;

	new_vec3(&terminus, 0, 0, 0);
	y = -1;
	while (++y < scene->wheight)
	{
		x = -1;
		while (++x < scene->wwidth)
		{
			new_vec3(&orientation, (x - scene->wwidth / 2.0) / scale, (y - scene->wheight / 2.0) / scale, focal_distance);
			new_ray(&ray, &terminus, &orientation);
			res_color = cast_ray(&t, &ray, objects, scene->nb_objs);
			if (!ray_at(&ray, t, &intersection))//todo: add ambience lighting for void
				apply_light(&res_color, calculate_lighting(&intersection, objects, scene));
			mlx_pixel_put_buf(&gc->img, x, scene->wheight - y, res_color);
		}
	}
	mlx_put_image_to_window(gc->mlx, gc->win, gc->img.img, 0, 0);
}
