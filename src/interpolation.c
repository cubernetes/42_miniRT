/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpolation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:55:33 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/18 02:12:54 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

void	color_to_vec3(t_vec3 *vec, t_color color)
{
	vec->r = get_red(&color);
	vec->g = get_green(&color);
	vec->b = get_blue(&color);
}

void	vec3_to_color(t_vec3 *vec, t_color *color)
{
	set_red(color, (unsigned int)vec->r);
	set_green(color, (unsigned int)vec->g);
	set_blue(color, (unsigned int)vec->b);
}

void	interpolation(t_gc *gc, unsigned int width, unsigned int height, unsigned int resolution)
{
	unsigned int	x;
	unsigned int	y;
	t_color			res_color;
	t_vec3			top_left;
	t_vec3			top_right;
	t_vec3			bot_left;
	t_vec3			bot_right;
	t_vec3			top_left_tmp;
	t_vec3			top_right_tmp;
	t_vec3			bot_left_tmp;
	t_vec3			bot_right_tmp;
	t_vec3			top_interpolation;
	t_vec3			bot_interpolation;
	t_vec3			res_tmp;

	y = 0;
	while (y + resolution - 1 < height)
	{
		x = 0;
		while (x + resolution - 1 < width)
		{
			color_to_vec3(&top_left, mlx_pixel_get_buf(&gc->img2, (int)x, (int)y));
			if (x + 2 * resolution - 1 <= width)
				color_to_vec3(&top_right, mlx_pixel_get_buf(&gc->img2, (int)(x + resolution), (int)y));
			else
				color_to_vec3(&top_right, mlx_pixel_get_buf(&gc->img2, (int)x, (int)y));
			if (y + 2 * resolution - 1 <= height)
				color_to_vec3(&bot_left, mlx_pixel_get_buf(&gc->img2, (int)x, (int)(y + resolution)));
			else
				color_to_vec3(&bot_left, mlx_pixel_get_buf(&gc->img2, (int)x, (int)y));
			if (x + 2 * resolution - 1 <= width && y + 2 * resolution - 1 <= height)
				color_to_vec3(&bot_right, mlx_pixel_get_buf(&gc->img2, (int)(x + resolution), (int)(y + resolution)));
			else
				color_to_vec3(&bot_right, mlx_pixel_get_buf(&gc->img2, (int)x, (int)y));
			for (unsigned int i = 0; i < resolution; i++)
			{
				for (unsigned int j = 0; j < resolution; j++)
				{
					copy_vec3(&top_left_tmp, &top_left);
					sc_mult_vec3(&top_left_tmp, (double) (resolution - j) / (double) resolution);
					copy_vec3(&top_right_tmp, &top_right);
					sc_mult_vec3(&top_right_tmp, (double) j / (double) resolution);
					copy_vec3(&top_interpolation, &top_left_tmp);
					add_vec3(&top_interpolation, &top_right_tmp);
					sc_mult_vec3(&top_interpolation, (double) (resolution - i) / (double) resolution);
					copy_vec3(&bot_left_tmp, &bot_left);
					sc_mult_vec3(&bot_left_tmp, (double) (resolution - j) / (double) resolution);
					copy_vec3(&bot_right_tmp, &bot_right);
					sc_mult_vec3(&bot_right_tmp, (double) j / (double) resolution);
					copy_vec3(&bot_interpolation, &bot_left_tmp);
					add_vec3(&bot_interpolation, &bot_right_tmp);
					sc_mult_vec3(&bot_interpolation, (double) i / (double) resolution);
					copy_vec3(&res_tmp, &top_interpolation);
					add_vec3(&res_tmp, &bot_interpolation);
					vec3_to_color(&res_tmp, &res_color);
					mlx_pixel_put_buf(&gc->img2, (int) (x + j), (int) (y + i), res_color);
				}
			}
			x += resolution;
		}
		y += resolution;
	}
}
