#include "miniRT.h"
#include "libft.h"
#include "mlx.h"

#include "float.h"
#include <math.h>

void	calculate_norm(t_hit *hit)
{
	t_vec3	norm_base;
	t_vec3	temp;
	t_ray	ray;
	double	norm_len_squared;

	if (hit->object->type == TOK_PLANE)
		copy_vec3(&hit->norm, hit->object->plane.norm);
	else if (hit->object->type == TOK_SPHERE)
	{
		copy_vec3(&hit->norm, &hit->point);
		substract_vec3(&hit->norm, hit->object->sphere.center);
	}
	else if (hit->object->type == TOK_CYLINDER)
	{
		new_ray(&ray, hit->object->cylinder.center, hit->object->cylinder.axis);
		norm_point_to_line(&norm_base, &hit->point, &ray);
		copy_vec3(&hit->norm, &hit->point);
		substract_vec3(&hit->norm, &norm_base);
		norm_len_squared = length_squared_vec3(&hit->norm);
		if (hit->object->cylinder.radius * hit->object->cylinder.radius - norm_len_squared > EPSILON2)
		{
			copy_vec3(&temp, &hit->point);
			substract_vec3(&temp, hit->object->cylinder.base_bot);
			copy_vec3(&hit->norm, hit->object->cylinder.axis);
			if (fabs(norm_len_squared - length_squared_vec3(&temp)) > EPSILON2)
				sc_mult_vec3(&hit->norm, -1);
		}
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
	{
		if (scene->objects[i]->type == TOK_PLANE)
			intersection_plane(&hit->t, &scene->objects[i]->plane, ray);
		else if (scene->objects[i]->type == TOK_SPHERE)
			intersection_sphere(&hit->t, &scene->objects[i]->sphere, ray);
		else if (scene->objects[i]->type == TOK_CYLINDER)
			intersection_cylinder(&hit->t, &scene->objects[i]->cylinder, ray);
		if ((hit->t > 0) && (hit->t < old_t || old_t == NO_ROOTS))
		{
			old_t = hit->t;
			hit->object = scene->objects[i];
			hit->color = scene->objects[i]->color;
		}
	}
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

/* T = C + C_l * (vw / (2 * tan(theta / 2)) - Cr * vw / 2 + Cu * vh / 2 */

/* rebase_vec3(t_vec3 *this, t_vec3 **new_basis); */
void	init_viewport_params(t_scene *scene, t_vec3 *terminus)
{
	t_vec3	scaled_up;
	t_vec3	scaled_right;
	t_vec3	scaled_view;
	double	focal_distance;

	copy_vec3(&scene->viewport->down_step, &scene->camera->up);
	sc_mult_vec3(&scene->viewport->down_step, -1);
	copy_vec3(&scene->viewport->right_step, &scene->camera->right);
	copy_vec3(&scene->viewport->top_left, terminus);
	copy_vec3(&scaled_view, &scene->camera->dir);
	focal_distance = scene->window_width / (2 * tan(scene->fov * PI / 360));
	sc_mult_vec3(&scaled_view, focal_distance);
	copy_vec3(&scaled_right, &scene->camera->right);
	sc_mult_vec3(&scaled_right, scene->window_width / 2.0);
	copy_vec3(&scaled_up, &scene->camera->up);
	sc_mult_vec3(&scaled_up, scene->window_height / 2.0);
	add_vec3(&scene->viewport->top_left, &scaled_view);
	substract_vec3(&scene->viewport->top_left, &scaled_right);
	add_vec3(&scene->viewport->top_left, &scaled_up);
	substract_vec3(&scene->viewport->top_left, &scene->camera->pos);
}

/* todo: change to FOV */
void	render(t_gc *gc, t_scene *scene)
{
	int				x;
	int				y;
	t_ray			ray;
	t_vec3			terminus;
	t_hit			hit;
	t_vec3			row_start_vec;
	t_vec3			pixel;
	int				counter;

	counter = 0;
	(void)counter; // todo: counter not used
	copy_vec3(scene->lights[0]->point, &scene->camera->pos);
	copy_vec3(&terminus, &scene->camera->pos);
	init_viewport_params(scene, &terminus);
	y = -1;
	copy_vec3(&row_start_vec, &scene->viewport->top_left);
	while (++y < scene->window_height)
	{
		x = -1;
		copy_vec3(&pixel, &row_start_vec);
		while (++x < scene->window_width)
		{
			new_ray(&ray, &terminus, &pixel);
			if (!cast_ray(&hit, &ray, scene))
			{
				if (hit.object->type == TOK_CYLINDER)
					counter++;
				apply_light(&(hit.color), calculate_lighting(&hit, scene));
			}
			mlx_pixel_put_buf(&gc->img, x, scene->window_height - y, hit.color);
			add_vec3(&pixel, &scene->viewport->right_step);
		}
		add_vec3(&row_start_vec, &scene->viewport->down_step);
	}
	mlx_put_image_to_window(gc->mlx, gc->win, gc->img.img, 0, 0);
}
