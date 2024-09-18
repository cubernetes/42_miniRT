/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   antialiasing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 21:02:35 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/12 21:02:35 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include <stdio.h>
#include <math.h>

static int	mod(int a, int b)
{
	const int	r = a % b;

	if (r < 0)
		return (r + b);
	return (r);
}

void	apply_random_antialiasing(t_gc *gc, int width, int height)
{
	int			x;
	int			y;
	int			i;
	int			rand;
	t_color		res_color;
	t_color		tmp_color;
	const int	sample_amount = 4;

	y = -1;
	while (++y < height)
	{
		x = -1;
		while (++x < width)
		{
			res_color = 0;
			i = -1;
			while (++i < sample_amount)
			{
				rand = ft_rand();
				tmp_color = mlx_pixel_get_buf(&gc->img, x + (mod((rand / 3), 3)) - 1, y + (mod(rand, 3)) - 1);
				set_red(&res_color, get_red(&res_color) + get_red(&tmp_color) / sample_amount);
				set_green(&res_color, get_green(&res_color) + get_green(&tmp_color) / sample_amount);
				set_blue(&res_color, get_blue(&res_color) + get_blue(&tmp_color) / sample_amount);
			}
			mlx_pixel_put_buf(&gc->img2, x, y, res_color);
		}
	}
}

static int	edge_detection(t_rt_img *img, int x, int y, int resolution)
{
	t_vec3			grad_x;
	t_vec3			grad_y;
	const int	kernel_x[3][3] = {{1, 0, -1},
							{2, 0, -2},
							{1, 0, -1}};
	const int	kernel_y[3][3] = {{1, 2, 1},
								{0, 0, 0},
								{-1, -2, -1}};

	new_vec3(&grad_x, 0, 0, 0);
	new_vec3(&grad_y, 0, 0, 0);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			t_color	temp = mlx_pixel_get_buf(img, x + (i - 1) * resolution, y + (j - 1) * resolution);
			grad_x.x += kernel_x[2 - i][2 - j] * (int)get_red(&temp);
			grad_x.y += kernel_x[2 - i][2 - j] * (int)get_green(&temp);
			grad_x.z += kernel_x[2 - i][2 - j] * (int)get_blue(&temp);
			grad_y.x += kernel_y[2 - i][2 - j] * (int)get_red(&temp);
			grad_y.y += kernel_y[2 - i][2 - j] * (int)get_green(&temp);
			grad_y.z += kernel_y[2 - i][2 - j] * (int)get_blue(&temp);
		}
	}
	const double len_sq_x = length_squared_vec3(&grad_x);
	const double len_sq_y = length_squared_vec3(&grad_y);
//	double grad_degree = atan2(sqrt(len_sq_y), sqrt(len_sq_x)) * 180 / PI;
//	if (grad_degree < 0.0)
//		grad_degree += 360.0;
	if (len_sq_x + len_sq_y < EPSILON)
		return (0);
//	printf("grad degree = %f\n", grad_degree);
	return (1);
}

void	apply_pattern_antialiasing(t_gc *gc, int width, int height, int resolution)
{
	int			x;
	int			y;
	t_color		res_color;
	t_color		tmp_color;

	y = 0;
	while (y + resolution - 1 < height)
	{
		x = 0;
		while (x + resolution - 1 < width)
		{
			if (x > resolution && y > resolution
				&& x + 2 * resolution - 1 < width
				&& y + 2 * resolution - 1 < height
				&& (edge_detection(&gc->img3, x, y, resolution)))
			{
				res_color = 0;
				for (int i = -1; i < 2; i++)
				{
					for (int j = -1; j < 2; j++)
					{
						tmp_color = mlx_pixel_get_buf(&gc->img, x + i * resolution, y + j * resolution);
						set_red(&res_color, get_red(&res_color) + get_red(&tmp_color) / 9);
						set_green(&res_color, get_green(&res_color) + get_green(&tmp_color) / 9);
						set_blue(&res_color, get_blue(&res_color) + get_blue(&tmp_color) / 9);
					}
				}
//				set_red(&res_color, 255);
//				set_green(&res_color, 255);
//				set_blue(&res_color, 255);
			}
			else
				res_color = mlx_pixel_get_buf(&gc->img, x, y);
//				res_color = 0;
			for (int i = 0; i < resolution; i++)
			{
				for (int j = 0; j < resolution; j++)
				{
					mlx_pixel_put_buf(&gc->img2, x + i, y + j, res_color);
				}
			}
			x += resolution;
		}
		y += resolution;
	}
}
