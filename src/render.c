#include "libft.h"
#include "miniRT.h"
#include "float.h"
#include "mlx.h"

#include <math.h>

#include <stdio.h> // TODO: remove

void	calculate_norm(t_hit *hit)
{
	t_vec3	norm_base;
	t_vec3	temp;
	t_ray	ray;
	double	norm_len_squared;

	if (hit->object->type == PLANE)
		copy_vec3(&hit->norm, hit->object->plane.norm);
	else if (hit->object->type == SPHERE)
	{
		copy_vec3(&hit->norm, &hit->point);
		substract_vec3(&hit->norm, hit->object->sphere.center);
	}
	else if (hit->object->type == CYLINDER)
	{
		new_ray(&ray, hit->object->cylinder.center, hit->object->cylinder.axis);
		norm_point_to_line(&norm_base, &hit->point, &ray);
		copy_vec3(&hit->norm, &hit->point);
		substract_vec3(&hit->norm, &norm_base);
		norm_len_squared = length_squared_vec3(&hit->norm);
		if (hit->object->cylinder.radius * hit->object->cylinder.radius - norm_len_squared > 0.000001)
		{
			copy_vec3(&temp, &hit->point);
			substract_vec3(&temp, hit->object->cylinder.base_bot);
			if (norm_len_squared - length_squared_vec3(&temp) > 0.000001)
				copy_vec3(&hit->norm, hit->object->cylinder.base_top);
			else
				copy_vec3(&hit->norm, hit->object->cylinder.base_bot);
		}
	}
}

/* todo: handle no objects edge case */
int	cast_ray(t_hit *hit, t_ray *ray, t_obj *objects, int nb_objs)
{
	int		i;
	double	old_t;

	i = -1;
	hit->t = NO_ROOTS;
	old_t = INFINITY;
	hit->color = 0x00000000;
	while (++i < nb_objs)
	{
		if (objects[i].type == PLANE)
			intersection_plane(&hit->t, &objects[i].plane, ray);
		else if (objects[i].type == SPHERE)
			intersection_sphere(&hit->t, &objects[i].sphere, ray);
		else if (objects[i].type == CYLINDER)
			intersection_cylinder(&hit->t, &objects[i].cylinder, ray);
		if ((hit->t > 0) && (hit->t < old_t || old_t == NO_ROOTS))
		{
			old_t = hit->t;
			hit->object = &(objects[i]);
			hit->color = objects[i].color;
		}
	}
	if (old_t == INFINITY)
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

/* todo: change to FOV */
void	render(t_gc *gc, t_scene *scene, t_obj *objects)
{
	int				x;
	int				y;
	t_ray			ray;
	t_vec3			terminus;
	t_vec3			orientation;
	t_hit			hit;
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
			if (!cast_ray(&hit, &ray, objects, scene->nb_objs))
				apply_light(&(hit.color), calculate_lighting(&hit, objects, scene));
			mlx_pixel_put_buf(&gc->img, x, scene->wheight - y, hit.color);
		}
	}
	mlx_put_image_to_window(gc->mlx, gc->win, gc->img.img, 0, 0);
}
