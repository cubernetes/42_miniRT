/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sample_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:07:17 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 11:26:17 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "mlx.h"

void	render_pixel(t_rendering *r)
{
	if (((r->y * r->gc->scene->window_height) / r->resolution + r->x
			/ r->resolution - r->sample) % r->sample_size == 0)
	{
		new_ray(&r->ray, &r->terminus, &r->pixel);
		r->tmp_color = 0;
		if (!cast_ray(&r->hit, &r->ray, r->gc->scene))
		{
			r->tmp_color = r->hit.color;
			apply_light(&r->hit.color,
				calculate_lighting(&r->hit, r->gc->scene));
		}
		r->i = -1;
		while (++r->i < r->resolution)
		{
			r->j = -1;
			while (++r->j < r->resolution)
				(mlx_pixel_put_buf(&r->gc->img, r->x + r->j, r->y + r->i,
						r->hit.color), mlx_pixel_put_buf(&r->gc->img3,
						r->x + r->j, r->y + r->i, r->tmp_color));
		}
	}
	r->j = -1;
	while (++r->j < r->resolution)
		add_vec3(&r->pixel, &r->gc->scene->viewport->right_step);
	r->x += r->resolution;
}

void	handle_antialiasing(t_gc *gc, int resolution)
{
	if (gc->antialiasing)
	{
		apply_pattern_antialiasing(gc, gc->scene->window_width,
			gc->scene->window_height, resolution);
		if (gc->interpolation)
			interpolation(gc, (unsigned int)gc->scene->window_width,
				(unsigned int)gc->scene->window_height,
				(unsigned int)resolution);
		render_cursor(gc);
		mlx_put_image_to_window(gc->mlx, gc->win, gc->img2.img, 0, 0);
	}
	else
	{
		render_cursor(gc);
		mlx_put_image_to_window(gc->mlx, gc->win, gc->img.img, 0, 0);
	}
}

void	sample_frame(t_gc *gc, int resolution, int sample,
	int sample_size)
{
	t_rendering	*r;

	r = &(t_rendering){.gc = gc, .resolution = resolution, .sample = sample,
		.sample_size = sample_size};
	init_render(gc->scene, &r->terminus);
	copy_vec3(&r->row_start_vec, &gc->scene->viewport->top_left);
	while (r->y + resolution - 1 < gc->scene->window_height)
	{
		r->x = 0;
		copy_vec3(&r->pixel, &r->row_start_vec);
		while (r->x + resolution - 1 < gc->scene->window_width)
			render_pixel(r);
		r->i = -1;
		while (++r->i < resolution)
			add_vec3(&r->row_start_vec, &gc->scene->viewport->down_step);
		r->y += resolution;
	}
	handle_antialiasing(gc, resolution);
	mlx_do_sync(gc->mlx);
}
