/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 02:59:34 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/12 01:23:44 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include "mlx.h"

#include "float.h"
#include <math.h>
#include <stdio.h>

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

void	init_render(t_scene *scene, t_vec3 *terminus)
{
	copy_vec3(scene->lights[0]->point, &scene->camera->pos);
	copy_vec3(terminus, &scene->camera->pos);
	init_viewport_params(scene, terminus);
}

void	render(t_gc *gc, t_scene *scene, int resolution, int sample,
	int sample_size)
{
	int		i[4];
	t_ray	ray;
	t_vec3	terminus;
	t_hit	hit;
	t_vec3	vec[2];
	t_vec3	tmp_pixel;
	t_color	tmp_color;

	init_render(scene, &terminus);
	i[Y] = 0;
	copy_vec3(&vec[ROW_START_VEC], &scene->viewport->top_left);
	while (i[Y] + resolution - 1 < scene->window_height)
	{
		i[X] = 0;
		copy_vec3(&vec[PIXEL], &vec[ROW_START_VEC]);
		while (i[X] + resolution - 1 < scene->window_width)
		{
			if (((i[Y] * scene->window_height) / resolution + i[X] / resolution - sample)
				% sample_size == 0)
			{
				copy_vec3(&tmp_pixel, &vec[PIXEL]);
				unit_vec3(&tmp_pixel);
				new_ray(&ray, &terminus, &tmp_pixel);
				tmp_color = 0;
				if (!cast_ray(&hit, &ray, scene))
				{
					tmp_color = hit.color;
					apply_light(&(hit.color), calculate_lighting(&hit, scene));
				}
				i[I] = -1;
				while (++(i[I]) < resolution)
				{
					i[J] = -1;
					while (++(i[J]) < resolution)
					{
						mlx_pixel_put_buf(&gc->img, i[X] + i[J], i[Y] + i[I],
							hit.color);
						mlx_pixel_put_buf(&gc->img3, i[X] + i[J], i[Y] + i[I],
							tmp_color);
					}
				}
			}
			i[J] = -1;
			while (++(i[J]) < resolution)
				add_vec3(&vec[PIXEL], &scene->viewport->right_step);
			i[X] += resolution;
		}
		i[I] = -1;
		while (++(i[I]) < resolution)
			add_vec3(&vec[ROW_START_VEC], &scene->viewport->down_step);
		i[Y] += resolution;
	}
	if (gc->antialiasing)
	{
		apply_pattern_antialiasing(gc, scene->window_width, scene->window_height, resolution);
		mlx_put_image_to_window(gc->mlx, gc->win, gc->img2.img, 0, 0);
	}
	else
		mlx_put_image_to_window(gc->mlx, gc->win, gc->img.img, 0, 0);
	mlx_do_sync(gc->mlx);
}
