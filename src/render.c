/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 23:05:03 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/23 05:19:03 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "float.h"
#include "mlx.h"
#include <stdlib.h>

/* todo: handle no objects edge case */
t_color	cast_ray(double *t, t_ray *ray, t_obj *objects, int nb_objs)
{
	int		i;
	double	old_t;
	t_color	res_color;

	i = -1;
	*t = NO_ROOTS;
	old_t = DBL_MAX;
	res_color = 0x00000000;
	while (++i < nb_objs)
	{
		if (objects[i].type == PLANE)
			intersection_plane(t, &objects[i].plane, ray);
		else if (objects[i].type == SPHERE)
			intersection_sphere(t, &objects[i].sphere, ray);
		else if (objects[i].type == CYLINDER)
			intersection_cylinder(t, &objects[i].cylinder, ray);
		if (*t >= 0 && (*t < old_t || old_t == NO_ROOTS))
		{
			old_t = *t;
			res_color = objects[i].color;
		}
	}
	return (res_color);
}

/* todo: change to FOV */
void	render(t_gc *gc, t_scene *scene, t_obj *objects)
{
	int				x;
	int				y;
	t_color			res_color;
	t_ray			ray;
	t_vec3			terminus;
	t_vec3			orientation;
	double			t;
	const double	scale = 4.0;
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
			apply_light(&res_color, scene->amb_light);
			mlx_pixel_put_buf(&gc->img, x, scene->wheight - y, res_color);
		}
	}
	mlx_put_image_to_window(gc->mlx, gc->win, gc->img.img, 0, 0);
}

/* todo: make array of pointers instead of normal array */
/* todo: use vec3 structs instead of pointers to vec3 structs,
 * reduces fragmentation and the number of calls to (ft_)malloc
 */
int	parse_input(t_obj **objects, t_scene *scene)
{
	const double	radius = 5;

	scene->amb_light = 0.5;
	scene->nb_objs = 3;
	*objects = ft_malloc(sizeof(t_obj) * (size_t)scene->nb_objs);
	(*objects)[0].type = SPHERE;
	(*objects)[0].color = 0x00da2b27;
	(*objects)[0].sphere.center = ft_malloc(sizeof(t_vec3));
	new_vec3((*objects)[0].sphere.center, 0, 0, -10);
	new_sphere(&(*objects)[0].sphere, (*objects)[0].sphere.center, radius);
	(*objects)[1].type = PLANE;
	(*objects)[1].color = 0x00ce9d52;
	(*objects)[1].plane.norm = ft_malloc(sizeof(t_vec3));
	new_vec3((*objects)[1].plane.norm, 0, 1, 0);
	(*objects)[1].plane.point = ft_malloc(sizeof(t_vec3));
	new_vec3((*objects)[1].plane.point, 0, -29, 0);
	new_plane(&(*objects)[1].plane, (*objects)[1].plane.point, (*objects)[1].plane.norm);
	(*objects)[2].type = PLANE;
	(*objects)[2].color = 0x00f68656;
	(*objects)[2].plane.norm = ft_malloc(sizeof(t_vec3));
	new_vec3((*objects)[2].plane.norm, 0, 1, 0);
	(*objects)[2].plane.point = ft_malloc(sizeof(t_vec3));
	new_vec3((*objects)[2].plane.point, 0, 29, 0);
	new_plane(&(*objects)[2].plane, (*objects)[2].plane.point, (*objects)[2].plane.norm);
	return (EXIT_SUCCESS);
}
