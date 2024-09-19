/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpolation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:55:33 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/19 11:56:47 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	interpolate_top_left_right(t_interpolation *p)
{
	copy_vec3(&p->top_left_tmp, &p->top_left);
	sc_mult_vec3(&p->top_left_tmp, (double)(p->resolution - p->j)
		/ (double)p->resolution);
	copy_vec3(&p->top_right_tmp, &p->top_right);
	sc_mult_vec3(&p->top_right_tmp, (double)p->j / (double)p->resolution);
	copy_vec3(&p->top_interpolation, &p->top_left_tmp);
	add_vec3(&p->top_interpolation, &p->top_right_tmp);
	sc_mult_vec3(&p->top_interpolation, (double)(p->resolution - p->i)
		/ (double)p->resolution);
}

static void	interpolate_bot_left_right(t_interpolation *p)
{
	copy_vec3(&p->bot_left_tmp, &p->bot_left);
	sc_mult_vec3(&p->bot_left_tmp, (double)(p->resolution - p->j)
		/ (double)p->resolution);
	copy_vec3(&p->bot_right_tmp, &p->bot_right);
	sc_mult_vec3(&p->bot_right_tmp, (double)p->j / (double)p->resolution);
	copy_vec3(&p->bot_interpolation, &p->bot_left_tmp);
	add_vec3(&p->bot_interpolation, &p->bot_right_tmp);
	sc_mult_vec3(&p->bot_interpolation, (double)p->i / (double)p->resolution);
}

static void	interpolate_pixel(t_interpolation *p)
{
	while (++p->i < p->resolution)
	{
		p->j = (unsigned int)(-1);
		while (++p->j < p->resolution)
		{
			interpolate_top_left_right(p);
			interpolate_bot_left_right(p);
			copy_vec3(&p->res_tmp, &p->top_interpolation);
			add_vec3(&p->res_tmp, &p->bot_interpolation);
			vec3_to_color(&p->res_tmp, &p->res_color);
			mlx_pixel_put_buf(&p->gc->img2, (int)(p->x + p->j),
				(int)(p->y + p->i), p->res_color);
		}
	}
}

static void	inspect_surrounding_pixels(t_interpolation *p)
{
	color_to_vec3(&p->top_left,
		mlx_pixel_get_buf(&p->gc->img2, (int)p->x, (int)p->y));
	if (p->x + 2 * p->resolution - 1 <= p->width)
		color_to_vec3(&p->top_right, mlx_pixel_get_buf(&p->gc->img2,
				(int)(p->x + p->resolution), (int)p->y));
	else
		color_to_vec3(&p->top_right, mlx_pixel_get_buf(&p->gc->img2,
				(int)p->x, (int)p->y));
	if (p->y + 2 * p->resolution - 1 <= p->height)
		color_to_vec3(&p->bot_left, mlx_pixel_get_buf(&p->gc->img2,
				(int)p->x, (int)(p->y + p->resolution)));
	else
		color_to_vec3(&p->bot_left, mlx_pixel_get_buf(&p->gc->img2,
				(int)p->x, (int)p->y));
	if (p->x + 2 * p->resolution - 1 <= p->width && p->y + 2 * p->resolution - 1
		<= p->height)
		color_to_vec3(&p->bot_right, mlx_pixel_get_buf(&p->gc->img2,
				(int)(p->x + p->resolution), (int)(p->y + p->resolution)));
	else
		color_to_vec3(&p->bot_right, mlx_pixel_get_buf(&p->gc->img2,
				(int)p->x, (int)p->y));
}

void	interpolation(t_gc *gc, unsigned int width, unsigned int height,
	unsigned int resolution)
{
	t_interpolation	*p;

	p = &(t_interpolation){.gc = gc, .width = width, .height = height,
		.resolution = resolution};
	p->y = 0;
	while (p->y + resolution - 1 < height)
	{
		p->x = 0;
		while (p->x + resolution - 1 < width)
		{
			inspect_surrounding_pixels(p);
			p->i = (unsigned int)(-1);
			interpolate_pixel(p);
			p->x += resolution;
		}
		p->y += resolution;
	}
}
